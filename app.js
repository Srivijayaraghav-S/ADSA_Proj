const express = require('express');
const bodyParser = require('body-parser');
const { spawn } = require('child_process');

const app = express();
app.use(bodyParser.urlencoded({ extended: true }));

// Route handler for the POST request to /process_input endpoint
app.post('/process_input', (req, res) => {
  const inputText = req.body.input_text;

  // Spawn a child process to execute the C++ program
  const charCountProcess = spawn('./charcount', [inputText]);

  let outputData = '';

  // Capture the output data from the child process
  charCountProcess.stdout.on('data', (data) => {
    outputData += data;
  });

  // Handle the close event of the child process
  charCountProcess.on('close', (code) => {
    console.log(`Child process exited with code ${code}`);

    // Parse the output data as JSON
    const structArray = JSON.parse(outputData);

    // Send the struct array as the response to the client
    res.send(structArray);
  });
});

app.listen(3000, () => {
  console.log('Server running on http://localhost:3000');
});
