#ifndef EXTRACTZIP_H
#define EXTRACTZIP_H

class QString;
class QStringList;

/**
 * Extract files from a ZIP archive.
 *
 * @param   path                ZIP file path.
 * @param   excludedExtensions  Files in the ZIP with matching extensions will
 *                              not be extracted. Items in this list are case
 *                              insensitive, and start with ".".
 * @return  True (succes) or false (failure).
 */
bool extractZip(const QString &path, const QStringList &excludedExtensions);

#endif // EXTRACTZIP_H
