document.addEventListener('DOMContentLoaded', function() {
  document.getElementById('myForm').addEventListener('submit', function(event) {
    event.preventDefault();
    var inputText = document.getElementById('inputText').value;
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (xhr.readyState === XMLHttpRequest.DONE) {
        if (xhr.status === 200) {
          var response = JSON.parse(xhr.responseText);
          displayOutput(response);
        } else {
          console.error('Error:', xhr.status);
        }
      }
    };
    xhr.open('GET', '/processInput?text=' + encodeURIComponent(inputText));
    xhr.send();
  });
});

function displayOutput(data) {
  var outputDiv = document.getElementById('output');
  outputDiv.innerHTML = '';
  data.forEach(function(item) {
    var word = document.createElement('span');
    word.textContent = item.word;
    var size = document.createElement('span');
    size.textContent = item.size;
    var itemDiv = document.createElement('div');
    itemDiv.appendChild(word);
    itemDiv.appendChild(document.createTextNode(' ('));
    itemDiv.appendChild(size);
    itemDiv.appendChild(document.createTextNode(')'));
    outputDiv.appendChild(itemDiv);
  });
}
