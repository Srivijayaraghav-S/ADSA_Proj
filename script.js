function submitForm(event) {
    event.preventDefault();
    const input = document.getElementById('input').value;
    console.log(input);
    const minPrice = parseFloat(document.getElementById('minPrice').value);
    console.log(minPrice);
    const maxPrice = parseFloat(document.getElementById('maxPrice').value);
    console.log(maxPrice);
    const minRating = parseFloat(document.getElementById('minRating').value);
    console.log(minRating);
    const maxRating = parseFloat(document.getElementById('maxRating').value);
    console.log(maxRating);
    fetch('/search_algo', {
      method: 'POST',
      body: JSON.stringify({ input, min_price: minPrice, max_price: maxPrice, min_rating: minRating, max_rating: maxRating }),
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
    document.getElementById('minPrice').value = '';
    document.getElementById('maxPrice').value = '';
    document.getElementById('minRating').value = '';
    document.getElementById('maxRating').value = '';
    console.clear();
}

const form = document.querySelector('form');
form.addEventListener('submit', submitForm);
