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

app.post('/charcount', (req, res) => {
  const input = req.body.input;
  const charcount = spawn('./charcount', [input]);
  let output = '';
  charcount.stdout.on('data', (data) => {
    output += data.toString();
  });
  charcount.on('close', (code) => {
    if (code !== 0) {
      res.status(500).send(`charcount exited with code ${code}`);
    } else {
      try {
        const counts = JSON.parse(output);
        res.json(counts);
      } catch (error) {
        res.status(500).send(`Failed to parse charcount output: ${error}`);
      }
    }
  });
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
