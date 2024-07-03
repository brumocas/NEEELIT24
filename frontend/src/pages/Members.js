import React from "react";
import "./Pages.css"; // Import your CSS file

function MembersPage() {
    return (
        <div className="neelitoFreundenPageBackground">
            <div className="contentMembers">
                <h1 className="pageTitleMembers">Neeelito Freunden</h1>
                <div className="imageContainer">
                    <div className="imageItem">
                        <img src="images/Rabinho.png" alt="Image 1" />
                        <div className="description">
                            <p>Tenho medo de mulheres!</p>
                        </div>
                        <p className="imageLabel">Kiko</p>
                    </div>
                    <div className="imageItem">
                        <img src="images/CrunoBosta.png" alt="Image 2" />
                        <div className="description">
                            <p>Enfermeiras: wifey material</p>
                        </div>
                        <p className="imageLabel">Zinho</p>
                    </div>
                    <div className="imageItem">
                        <img src="images/Ropes.png" alt="Image 3" />
                        <div className="description">
                            <p>"Não sou corno!"</p>
                        </div>
                        <p className="imageLabel">Lindo</p>
                    </div>
                    <div className="imageItem">
                        <img src="images/Bala.png" alt="Image 4" />
                        <div className="description">
                            <p>Eu tentei...</p>
                        </div>
                        <p className="imageLabel">E Branco</p>
                    </div>
                </div>
            </div>
        </div>
    );
}

export default MembersPage;
