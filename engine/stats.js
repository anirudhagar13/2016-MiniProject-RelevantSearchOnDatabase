function init()
{
    window.addEventListener('storage',present,false);
    d1 = document.getElementById('time');
    d2 = document.getElementById('number');
    d3 = document.getElementById('box');
    d4 = document.getElementById('result');
    d5 = document.createElement('div');
    d5.className = "tuple";
    d6 = document.createElement('div');
    d6.className = "info";

    present();
}

function present()
{
    fetch_time = localStorage.getItem('fetch_time');
    k_box_size = localStorage.getItem('k_box_size');
    total_size = localStorage.getItem('total_size');
    tuple_id = JSON.parse(localStorage['tuple_id']);
    tuple = JSON.parse(localStorage['tuple']);
    tuple_score = JSON.parse(localStorage['tuple_score']);

    fixes();
}

function fixes()
{
    d1.innerHTML += "0." + fetch_time;
    d2.innerHTML += total_size;
    d3.innerHTML += k_box_size;

    for(i in tuple_id)
    {
        temp = d5.cloneNode(true);
        temp.innerHTML = tuple[i];

        temp1 = d6.cloneNode(true);
        temp1.innerHTML = "Tuple No : " + tuple_id[i]
                        + "  ;  " + "Relevancy : " + tuple_score[i];

        temp2 = document.createElement('br');

        d4.appendChild(temp);
        d4.appendChild(temp1);
        d4.appendChild(temp2);
    }
}
