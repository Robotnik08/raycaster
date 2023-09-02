let map = [];
const mapHtml = document.getElementById('map');

let mapWidth = 0;
let mapHeight = 0;

let selectedType = 0;
let wallColours = [
    '#000000',
    '#ffffff',
    '#ff0000',
    '#00ff00',
    '#0000ff',
    '#ffff00',
];

function changeType (id) {
    selectedType = id;
}

function drawMap () {
    mapHtml.innerHTML = '';
    for (let y = 0; y < mapHeight; y++) {
        const row = document.createElement('div');
        row.classList.add('row');
        for (let x = 0; x < mapWidth; x++) {
            const cell = document.createElement('div');
            cell.classList.add('cell');
            cell.style.backgroundColor = wallColours[map[x + y * mapWidth] != -1 ? map[x + y * mapWidth] : 5];
            cell.setAttribute('onclick', 'setMapType(' + (x + y * mapWidth) +')');
            row.appendChild(cell);
        }
        mapHtml.appendChild(row);
    }
}

function setMapType (id) {
    map[id] = selectedType;
    drawMap();
}
function updateMapSize () {
    mapWidth = parseInt(document.getElementById('map-size-x').value);
    mapHeight = parseInt(document.getElementById('map-size-y').value);
    map.length = mapWidth * mapHeight;
    for (let i = 0; i < map.length; i++) {
        map[i] ??= 0;
    }
    drawMap();
}
updateMapSize();

function borderMap() {
    for (let y = 0; y < mapHeight; y++) {
        map[y * mapWidth] = selectedType;
        map[y * mapWidth + mapWidth - 1] = selectedType;
    }
    for (let x = 0; x < mapWidth; x++) {
        map[x] = selectedType;
        map[(mapHeight - 1) * mapWidth + x] = selectedType;
    }
    drawMap();
}

function convertToCarray () {
    let carray = 'int map[] = {\n\t';
    for (let i = 0; i < map.length; i++) {
        carray += map[i] + ', ';
        if (i % mapWidth == mapWidth - 1) {
            carray += '\n\t';
        }
    }
    carray += `${mapWidth}, ${mapHeight}\n};`;
    //alert prompt
    document.getElementById('result').innerHTML = carray;
}