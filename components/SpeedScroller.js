
var itmCount = 0;

function fillSections(listview) {
    console.debug("fill scroller sections");
    if(!listview || !listview.model) {
        console.debug("NO LISTMODEL");
        return;
    }
    var count = (typeof listview.model.count === 'undefined' ? listview.model.length : listview.model.count);
    console.debug("Got "+ count + " elements in model");
    itmCount = count;
    console.debug(itmCount + " items");
}

function getItemPositionFromRelative(position) {
    var index = Math.ceil(((itmCount)*position)/100);
    return index-1;
}

