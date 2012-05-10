.pragma library

var progressColors = new Object
progressColors.day = "#0000FF"
progressColors.night = "#CCCCFF"
progressColors.sand = "#6C541E"

var backgrounds = new Object
backgrounds.day = "#F7F7F7"
backgrounds.night = "#000009"
backgrounds.sand = "#EDC9AF"

var webThemes = new Object
webThemes.day = "document.body.style.background = '#FFFFFB';document.body.style.color = '#000000'"
webThemes.night = "document.body.style.background = '#000000';document.body.style.color = '#BEBEBE'"
webThemes.sand = "document.body.style.background = '#EDC9AF';document.body.style.color = '#000000'"

function background(style) {
    return backgrounds[style]
}

function progressColor(style) {
    return progressColors[style]
}

function webTheme(style) {
    return webThemes[style]
}
