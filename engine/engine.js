function init()
{
    bar = document.getElementById("query");
    box = document.getElementById("result_box");
    bar.addEventListener("keypress",search,false);
    bar.addEventListener("blur",hide,false);

}

function hide()
{
    box.style.display = "none";
}

function search(event)
{
    key_press = event.keyCode;

    if(key_press == 32)
    {
        box.style.display = "block";

        xhr = new XMLHttpRequest();
        xhr.onreadystatechange = processResults;

        xhr.open("GET", "http://localhost/cgi-bin/client.cgi?query="+bar.value, true);
        xhr.send();

    }
}

function drop_box()
{
    return results_rcvd.split("+>>>");
}

function processResults(event)
{
    // will fire four times at max
    if(xhr.readyState == 4 && xhr.status == 200)
    {
        // Read the data send by the server
        results_rcvd = xhr.responseText;

        drop_tuples = drop_box();

        //Adding tuples dynamically to box
        for(i in drop_tuples)
        {
            ln = document.createElement('a');
            ln.innerHTML = drop_tuples[i];
            box.appendChild(ln);
        }
    }
}
