import React from "react";
import "./Pages.css"; // Import your CSS file

function MazePage() {
    // Define a 12x12 grid
    const rows = 12;
    const cols = 12;

    // Create an array to store the grid cells
    const grid = [];

    // Fill the grid array with rows and columns
    for (let i = 0; i < rows; i++) {
        // Create an array to store the cells in the current row
        const row = [];

        for (let j = 0; j < cols; j++) {
            // Define the style for each cell
            const cellStyle = {
                border: "1px solid black",
                width: "50px",
                height: "50px",
                display: "inline-block",
                position: "relative",
            };

            // Add top, right, bottom, and left lines
            if (i === 0) cellStyle.borderTop = "4px solid black";
            if (j === cols - 1) cellStyle.borderRight = "4px solid black";
            if (i === rows - 1) cellStyle.borderBottom = "4px solid black";
            if (j === 0) cellStyle.borderLeft = "4px solid black";

            // Add a red background color to the cell
            // if it's painted red
            // Here, you can conditionally apply this style
            // based on your logic for painting cells red
            // if (cellIsPaintedRed) cellStyle.backgroundColor = "red";

            // Push the cell into the row array
            row.push(
                <div key={`${i}-${j}`} style={cellStyle}></div>
            );
        }

        // Push the row into the grid array
        grid.push(
            <div key={i} style={{ clear: "both" }}>
                {row}
            </div>
        );
    }

    return (
        <div className="mazePageBackground">
            <div className="contentMaze">
                <h1 className="pageTitleMaze">Maze</h1>
                <div className="mazeGrid">{grid}</div>
            </div>
        </div>
    );
}

export default MazePage;
