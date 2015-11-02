
var itmCount = 0;

function fillSections(listview) {
    if(!listview || !listview.model) {
        return;
    }
    var count = (typeof listview.model.count === 'undefined' ? listview.model.length : listview.model.count);
    itmCount = count;
}

function getItemPositionFromRelative(position) {
    var index = Math.ceil(((itmCount)*position)/100);
    return index-1;
}
