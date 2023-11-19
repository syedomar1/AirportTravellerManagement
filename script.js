function startSimulation() {
    const numPassengers = document.getElementById('numPassengers').value;
    const numTerminals = document.getElementById('numTerminals').value;

    // Send the input data to the server for simulation
    fetch('/simulate', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ numPassengers, numTerminals }),
    })
    .then(response => response.json())
    .then(data => {
        // Display the simulation output on the webpage
        const outputDiv = document.getElementById('output');
        outputDiv.innerHTML = `<h2>Simulation Output:</h2><pre>${JSON.stringify(data, null, 2)}</pre>`;
    })
    .catch(error => {
        console.error('Error during simulation:', error);
    });
}
