const express = require('express');
const multer = require('multer');
const path = require('path');
const { spawn } = require('child_process');
const fs = require('fs');

const app = express();
const upload = multer({ dest: 'uploads/' });

app.use(express.static('public'));

app.post('/compress', upload.single('file'), (req, res) => {
    const inputFilePath = path.join(__dirname, req.file.path);
    const outputFilePath = path.join(__dirname, 'compressed', req.file.filename + '.huf');

    // Spawn a child process to execute the Huffman compression program
    const huffmanProcess = spawn(path.join(__dirname, 'src', 'huffman'), [inputFilePath, outputFilePath]);

    huffmanProcess.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
    });

    huffmanProcess.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    huffmanProcess.on('close', (code) => {
        if (code !== 0) {
            return res.status(500).send('An error occurred during compression.');
        }

        res.download(outputFilePath, 'compressed.huf', (err) => {
            if (err) {
                console.error(err);
            }
            fs.unlinkSync(inputFilePath);
            fs.unlinkSync(outputFilePath);
        });
    });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
