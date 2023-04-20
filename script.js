function submitForm(event) {
    event.preventDefault();
    const input = document.getElementById('input').value;
    fetch('/charcount', {
      method: 'POST',
      body: JSON.stringify({ input }),
      headers: { 'Content-Type': 'application/json' }
    })
    .then(response => response.json())
    .then(counts => {
      const output = document.getElementById('output').querySelector('tbody');
      output.innerHTML = '';
      counts.forEach(count => {
        const row = document.createElement('tr');
        const charCell = document.createElement('td');
        const countCell = document.createElement('td');
        charCell.textContent = count.c;
        countCell.textContent = count.count;
        row.appendChild(charCell);
        row.appendChild(countCell);
        output.appendChild(row);
      });
    })
    .catch(error => console.error(error));
}

const form = document.querySelector('form');
form.addEventListener('submit', submitForm);
