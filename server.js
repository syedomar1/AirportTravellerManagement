const express = require('express');
const bodyParser = require('body-parser');
const app = express();
const port = 3000;

// Serve static files from the "public" directory
app.use(express.static('public'));

// Parse incoming JSON requests
app.use(bodyParser.json());

// Endpoint for simulation
app.post('/simulate', (req, res) => {
    // Implement the simulation logic here using the provided parameters
    const numPassengers = req.body.numPassengers;
    const numTerminals = req.body.numTerminals;

    // Replace this with your actual simulation logic
    const simulationOutput = {
        message: 'Simulation completed successfully!',
        numPassengers,
        numTerminals,
        // ... additional simulation output
    };

    res.json(simulationOutput);
});

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});
