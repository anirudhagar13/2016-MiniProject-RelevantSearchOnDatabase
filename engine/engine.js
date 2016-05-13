function init()
{
    bar = document.getElementById("query");
    box = document.getElementById("result_box");
    bar.addEventListener("keypress",search,false);
    bar.addEventListener("blur",hide,false);

}

function redirect()
{
    localStorage.setItem('fetch_time',fetch_time);
    localStorage.setItem('k_box_size',k_box_size);
    localStorage.setItem('total_size',total_size);
    localStorage['tuple_id'] = JSON.stringify(tuple_id);
    localStorage['tuple'] = JSON.stringify(tuple);
    localStorage['tuple_score'] = JSON.stringify(tuple_score);
    window.open("stats.html");
}

function hide()
{
    box.style.display = "none";
    bar.addEventListener("focus",show,false);
}

function show()
{
    box.style.display = "block";
}

function search(event)
{
    key_press = event.keyCode;

    if(key_press == 32)
    {
        box.style.display = "block";

        ajax_fetch();

    }
}

function ajax_fetch()
{
    xhr = new XMLHttpRequest();
    xhr.onreadystatechange = processResults;

    xhr.open("GET", "http://localhost/cgi-bin/client.cgi?query="+bar.value+"@", true);
    xhr.send();
}

function tuple_extraction()
{
    tuple_id = [];
    tuple_score = [];
    tuple = [];

    temp_splitter = results_rcvd.split("^#");
    fetch_time = temp_splitter[1];
    after_processing = temp_splitter[0];
    console.log(fetch_time);

    temp_splitter = after_processing.split("$#");
    k_box_size = temp_splitter[1];
    after_processing = temp_splitter[0];
    console.log(k_box_size);

    temp_splitter = after_processing.split("!#");
    total_size = temp_splitter[1];
    after_processing = temp_splitter[0];
    console.log(total_size);

    after_processing =  after_processing.split("\n");
    for(i in after_processing)
    {
        if(after_processing[i] != "")
        {
            temp_splitter = after_processing[i].split(" +>>> ");
            tuple_score.push(temp_splitter[0]);

            temp = temp_splitter[1];
            if(temp)
            {
                temp_splitter = temp.split("*&^");
                tuple_id.push(temp_splitter[1]);

                tuple.push(temp_splitter[0]);
            }
        }
    }

    return tuple;
}

function fill_drop_box()
{

    //Adding tuples dynamically to box
    box.innerHTML = '';
    for(i in tuple)
    {
        ln = document.createElement('a');
        ln.innerHTML = tuple[i];
        box.appendChild(ln);
    }
}

function processResults(event)
{
    // closes window on exit
    if(bar.value == "exit")
        window.close();

    if(xhr.readyState == 4 && xhr.status == 200)
    {
        // Read the data send by the server
        results_rcvd = xhr.responseText;

        tuple_extraction();
        fill_drop_box();
    }
}


