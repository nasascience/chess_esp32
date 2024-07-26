var ws = new WebSocket('ws://192.168.1.154:81');
const board = document.getElementById('board-play-computer');

// Initial positions for a chess game
const initialPositions = {
    a1: 'wr', b1: 'wn', c1: 'wb', d1: 'wq', e1: 'wk', f1: 'wb', g1: 'wn', h1: 'wr',
    a2: 'wp', b2: 'wp', c2: 'wp', d2: 'wp', e2: 'wp', f2: 'wp', g2: 'wp', h2: 'wp',
    a7: 'bp', b7: 'bp', c7: 'bp', d7: 'bp', e7: 'bp', f7: 'bp', g7: 'bp', h7: 'bp',
    a8: 'br', b8: 'bn', c8: 'bb', d8: 'bq', e8: 'bk', f8: 'bb', g8: 'bn', h8: 'br'
};

// Populate the initial state in Firebase
// Fill empty squares with null or an empty string
const emptySquares = [
    'a3', 'b3', 'c3', 'd3', 'e3', 'f3', 'g3', 'h3',
    'a4', 'b4', 'c4', 'd4', 'e4', 'f4', 'g4', 'h4',
    'a5', 'b5', 'c5', 'd5', 'e5', 'f5', 'g5', 'h5',
    'a6', 'b6', 'c6', 'd6', 'e6', 'f6', 'g6', 'h6'
];

function createChessBoard() {
    const squares = [
        'a8', 'b8', 'c8', 'd8', 'e8', 'f8', 'g8', 'h8',
        'a7', 'b7', 'c7', 'd7', 'e7', 'f7', 'g7', 'h7',
        'a6', 'b6', 'c6', 'd6', 'e6', 'f6', 'g6', 'h6',
        'a5', 'b5', 'c5', 'd5', 'e5', 'f5', 'g5', 'h5',
        'a4', 'b4', 'c4', 'd4', 'e4', 'f4', 'g4', 'h4',
        'a3', 'b3', 'c3', 'd3', 'e3', 'f3', 'g3', 'h3',
        'a2', 'b2', 'c2', 'd2', 'e2', 'f2', 'g2', 'h2',
        'a1', 'b1', 'c1', 'd1', 'e1', 'f1', 'g1', 'h1'
    ];

    squares.forEach((square, index) => {
        const div = document.createElement('div');
        div.setAttribute('draggable', "true")
        div.className = `piece square-${square}`;
        //div.ondrop = drop(this)
        div.id = square;
        board.appendChild(div);  // <div class="piece square-a1"></div>
    });
}

// Function to populate the board with pieces
function populateBoard(pieces) {
    console.log("pieces", pieces)
    for (const [square, piece] of Object.entries(pieces)) {
        console.log("square", square)
        console.log("piece", piece)
        const squareDiv = document.getElementById(square);
        console.log("squareDiv", squareDiv)
        if (squareDiv && piece) {
            squareDiv.classList.add(piece, 'piece');
            // Optionally add text to represent pieces
            // squareDiv.textContent = piece;
        } else if (squareDiv && !piece) {
            const id = squareDiv.getAttribute('id');
            squareDiv.className = `piece square-${id}`;
        }
    }
}

// Creates the initial state of the chess game in firebase placing the pieces where they should go
function createInitialState() {
    populateBoard(initialPositions);
}

function moveKnight() {
    writeUserData("g1", "");
    writeUserData("f3", "wn");
}

// Initialize the board
createChessBoard();
createInitialState()


ws.onmessage = function (event) {
    //console.log("data", event.data)
    if (event.data == "<0") {
        console.log("piece PRESSED")
    }
    // else if (event.data == "<0") {
    //     console.log("piece NOT pressed")
    // }
    // if (event.data === 'ButtonPressed') {
    //     var relayButton = document.getElementById('relayButton');
    //     if (relayButton.innerHTML === 'Relay ON') {
    //         relayButton.style.backgroundColor = 'red';
    //         relayButton.innerHTML = 'Relay OFF';
    //     } else {
    //         relayButton.style.backgroundColor = 'green';
    //         relayButton.innerHTML = 'Relay ON';
    //     }
    // }
};

// setTimeout(() => {
//     moveKnight();
// }, 1500)