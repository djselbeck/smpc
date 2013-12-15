
var sections = [];

function fillSections(listview) {
    sections = [];
    if(!listview || !listview.model) {
        return;
    }
    var count = listview.model.count;

    var currentSection = "";
    var currentItem;
    var sectionProperty = listview.section.property
    for(var i = 0; i < count; i++) {
        currentItem = listview.model.get(i);
        if(currentItem[sectionProperty] !== currentSection ) {
            currentSection = currentItem[sectionProperty];
            sections.push({index: i, value: currentSection});
        }
    }
}

function getSectionNameAtRelativePos(position) {
    var index = Math.ceil(((sections.length)*position)/100);
    return sections[index-1];
}

