#include <QFile>
#include <QDir>
#include <QDebug>

#include "extractzip.h"
#include "unzip/unzip.h"

#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

int doExtractCurrentFile(unzFile uf, const QStringList &excludedExtensions) {
    char fileNameInZip[MAXFILENAME];
    char *fileNameWithoutPath;
    char *p;
    int err = UNZ_OK;
    QFile *f = 0;
    QDir dir;
    void *buf;
    uInt bufSize;
    bool ret = true;
    unz_file_info64 fileInfo;

    err = unzGetCurrentFileInfo64(uf, &fileInfo, fileNameInZip, sizeof(fileNameInZip), NULL, 0, NULL, 0);
    if (err != UNZ_OK) {
        qCritical() << "doExtractCurrentFile: Error" << err << "in unzGetCurrentFileInfo";
        return err;
    }

    bufSize = WRITEBUFFERSIZE;
    buf = (void *)malloc(bufSize);
    if (buf == NULL) {
        qCritical() << "doExtractCurrentFile: Error allocating memory";
        return UNZ_INTERNALERROR;
    }

    p = fileNameWithoutPath = fileNameInZip;
    while ((*p) != '\0') {
        if (((*p) == '/') || ((*p) == '\\')) {
            fileNameWithoutPath = p + 1;
        }
        p++;
    }

    if ((*fileNameWithoutPath) == '\0') {
        dir.mkdir(fileNameInZip);
    } else {
        QString name(fileNameInZip);
        for (int i = 0; i < excludedExtensions.length(); i++) {
            if (name.endsWith(excludedExtensions[i], Qt::CaseInsensitive)) {
                free(buf);
                return UNZ_OK;
            }
        }

        const char *writeFileName;
        int skip = 0;

        writeFileName = fileNameInZip;

        err = unzOpenCurrentFilePassword(uf, 0);
        if (err != UNZ_OK) {
            qCritical() << "doExtractCurrentFile: Error" << err << "in unzOpenCurrentFilePassword";
            ret = false;
        }

        if ((skip == 0) && (err == UNZ_OK)) {
            f = new QFile(writeFileName);
            ret = f->open(QIODevice::WriteOnly);

            /* some zipfile don't contain directory alone before file */
            if (!ret && (fileNameWithoutPath != (char *)fileNameInZip)) {
                char c = *(fileNameWithoutPath-1);
                *(fileNameWithoutPath - 1) = '\0';
                dir.mkpath(writeFileName);
                *(fileNameWithoutPath - 1) = c;
                delete f;
                f = new QFile(writeFileName);
                ret = f->open(QIODevice::WriteOnly);
            }

            if (!ret) {
                qCritical() << "doExtractCurrentFile: Error opening" << writeFileName;
            }
        }

        if (ret) {
            do {
                err = unzReadCurrentFile(uf, buf, bufSize);
                if (err < 0) {
                    qCritical() << "doExtractCurrentFile: Error" << err << "in unzReadCurrentFile";
                    break;
                }
                if (err > 0) {
                    if (f->write((char *)buf, err) != err) {
                        qCritical() << "doExtractCurrentFile:" << "Error in writing extracted file";
                        err = UNZ_ERRNO;
                        break;
                    }
                }
            }
            while (err > 0);

            f->close();
        }

        if (err == UNZ_OK) {
            err = unzCloseCurrentFile(uf);
            if (err != UNZ_OK) {
                qCritical() << "doExtractCurrentFile: Error" << err << "with zipfile in unzCloseCurrentFile";
            }
        }
        else {
            unzCloseCurrentFile(uf); /* don't lose the error */
        }
    }

    delete f;
    free(buf);
    return err;
}

bool doExtract(unzFile uf, const QStringList &excludedExtensions) {
    uLong i;
    unz_global_info64 gi;
    int err;

    err = unzGetGlobalInfo64(uf, &gi);
    if (err != UNZ_OK) {
        qCritical() << "doExtract: Error" << err << "in unzGetGlobalInfo";
        return false;
    }

    for (i = 0; i < gi.number_entry; i++) {
        if (doExtractCurrentFile(uf, excludedExtensions) != UNZ_OK) {
            return false;
        }
        if ((i + 1) < gi.number_entry) {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK) {
                qCritical() << "doExtract: Error" << err << "in unzGoToNextFile";
                return false;
            }
        }
    }

    return true;
}

bool extractZip(const QString &zipFile, const QStringList &excludedExtensions) {
    unzFile uf;
    bool ret = false;

    uf = unzOpen64(zipFile.toUtf8().constData());
    if (uf) {
        ret = doExtract(uf, excludedExtensions);
        unzClose(uf);
    }
    return ret;
}
