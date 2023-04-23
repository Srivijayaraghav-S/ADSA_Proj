function submitForm(event) {
    event.preventDefault();
    const input = document.getElementById('input').value;
    fetch('/search_algo', {
      method: 'POST',
      body: JSON.stringify({ input }),
      headers: { 'Content-Type': 'application/json' }
    })
    .then(response => response.json())
    .then(finalResults => {
      console.log("hello");
      console.log(finalResults);
      const output = document.getElementById('output').querySelector('tbody');
      output.innerHTML = '';
      console.log("Hi");
      finalResults.forEach(finalResult => {
        const row = document.createElement('tr');
        const nameCell = document.createElement('td');
        const categoryCell = document.createElement('td');
        const priceCell = document.createElement('td');
        const descCell = document.createElement('td');
        const ratingCell = document.createElement('td');
        nameCell.textContent = finalResult.element.name;
        categoryCell.textContent = finalResult.element.category;
        priceCell.textContent = finalResult.element.price;
        descCell.textContent = finalResult.element.description;
        ratingCell.textContent = finalResult.element.rating;
        row.appendChild(nameCell);
        row.appendChild(categoryCell);
        row.appendChild(priceCell);
        row.appendChild(descCell);
        row.appendChild(ratingCell);
        output.appendChild(row);
        //console.log(finalResult);
      });
    })
    .catch(error => console.error(error));
    document.getElementById('input').value = '';
    console.clear();
}

const form = document.querySelector('form');
form.addEventListener('submit', submitForm);
