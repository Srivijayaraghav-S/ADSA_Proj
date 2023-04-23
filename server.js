const express = require('express');
const app = express();
const { spawn } = require('child_process');
const port = 3000;
const mime = require('mime');

app.use(express.static('opt_prod_search'));

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

app.use(express.json());

app.get('/script.js', (req, res) => {
  const filename = __dirname + '/script.js';
  const type = mime.getType(filename);
  res.setHeader('Content-Type', type);
  res.sendFile(filename);
});

app.post('/search_algo', (req, res) => {
  const input = req.body.input;
  const search_algo = spawn('./search_algo', [input]);
  let output = '';
  search_algo.stdout.on('data', (data) => {
    output += data.toString();
  });
  search_algo.on('close', (code) => {
    if (code !== 0) {
      res.status(500).send(`search_algo exited with code ${code}`);
    } else {
      try {
        const finalResults = JSON.parse(output);
        res.json(finalResults);
      } catch (error) {
        res.status(500).send(`Failed to parse search_algo output: ${error}`);
      }
    }
  });
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
