import React from "react";
import { Link } from "react-router-dom";
import "./Pages.css";

function HomePage() {
    return (
        <div className="homePageBackground">
            <div className="content">
                <h1 className="pageTitle">Hello, I am Neeelito el craque!</h1>
                <Link to="/Neeelito-Page" className="button">Lezz go!</Link>
                <Link to="/Members-Page" className="button">Neeelito Freunde</Link>
            </div>
        </div>
    );
}

export default HomePage;
