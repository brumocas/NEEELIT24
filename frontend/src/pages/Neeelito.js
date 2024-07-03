import React from "react";
import { Link } from "react-router-dom";
import "./Pages.css"; // Import your CSS file

function NeeelitoPage() {
    return (
        <div className="neelitoPageBackground">
            <div className="contentNeeelito">
                <h1 className="pageTitleNeeelito">Choose the algorithm</h1>
                <div className="buttonContainer">
                    <a href="http://192.168.232.57/" className="algorithmButton">Controls</a>
                    <Link to="/MazeMap-Page" className="algorithmButton">Maze Map</Link>
                </div>
            </div>
        </div>
    );
}

export default NeeelitoPage;
