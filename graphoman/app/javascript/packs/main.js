document.body.addEventListener("ajax:success", (event) => {
    const [data, status, xhr] = event.detail;
    let result_div = document.querySelector('#result');
    let log_div = document.querySelector('#log');
    result_div.innerText = "";

    let head = document.createElement("h2");
    head.id = "header-result";
    head.innerHTML = "Все построенные функции:";
    result_div.appendChild(head);

    var length = data.tables.length;
    if (length != 0){
        let table = document.createElement("table");
        table.id = "table-result";
        result.appendChild(table);
        let tr = document.createElement("tr");
        let th1 = document.createElement("th");
        let th2 = document.createElement("th");
        let th3 = document.createElement("th");
        tr.appendChild(th1);
        tr.appendChild(th2);
        tr.appendChild(th3);

        th1.innerHTML = "Функция";
        th2.innerHTML = "от";
        th3.innerHTML = "до";

        table.appendChild(tr);
        for(let i = 0; i < data.tables.length; ++i){
            let val = data.tables[0, i];
            let tr = document.createElement("tr");

            let td1 = document.createElement("td");
            let td2 = document.createElement("td");
            let td3 = document.createElement("td");
            let td4 = document.createElement("td");
            tr.appendChild(td1);
            tr.appendChild(td2);
            tr.appendChild(td3);
            tr.appendChild(td4);

            td1.innerHTML = val['func'];
            td2.innerHTML = val['lower_x'];
            td3.innerHTML = val['upper_x'];
            // td4.setAttribute("href", "/");
            table.appendChild(tr);
        }
    }else{
        result_div.innerHTML = "Ещё не было ни одной функции."
    }
    
    // var last_record = data.tables[0, length - 1];
    if (data.dataset.length != 0){
        var svg = d3.select("svg"),
                margin = {top: 30, right: 10, bottom: 30, left: 30},
                width = svg.attr("width") - margin.left - margin.right,
                height = svg.attr("height") - margin.top - margin.bottom;
        
        var xScale = d3.scaleLinear().domain([data.x_min, data.x_max]).range([0, width]),
            yScale = d3.scaleLinear().domain([data.y_min, data.y_max]).range([height, 0]);

        log_div.innerHTML = xScale.ticks(4);
        svg.append('text')
            .attr('x', width/2 + margin.left)
            .attr('y', margin.top/2)
            .attr('text-anchor', 'middle')
            .style('font-family', 'Helvetica')
            .style('font-size', 20)
            .text(data.func);
            
            // X label
        svg.append('text')
            .attr('x', width/2 + margin.left)
            .attr('y', height + margin.top + margin.bottom/2)
            .attr('text-anchor', 'middle')
            .style('font-family', 'Helvetica')
            .style('font-size', 12)
            .text('X');
            
            // Y label
        svg.append('text')
            .attr('text-anchor', 'middle')
            .attr('transform', 'translate(' + margin.left/2 + ',' + (height + margin.top)/2 + ')rotate(-90)')
            .style('font-family', 'Helvetica')
            .style('font-size', 12)
            .text('Y');
        
        var g = svg.append("g")
            .attr("transform", "translate(" + margin.top + "," + margin.left + ")");

        g.append("g")
            .attr("transform", "translate(0," + ((margin.top)/2) + ")")
            .call(d3.axisBottom(xScale));
        
        g.append("g")
            .attr("transform", "translate("+ ((margin.left)/2) + ", 0)")
            .call(d3.axisLeft(yScale));

        svg.append("g")
            .selectAll("dot")
            .data(data.dataset)
            .enter()
            .append("circle")
            .attr("cx", function (d) { return xScale(d[0]); } )
            .attr("cy", function (d) { return yScale(d[1]); } )
            .attr("r", 1)
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
            .style("fill", "#CC0000");

        var line = d3.line()
            .x(function(d) { return xScale(d[0]); }) 
            .y(function(d) { return yScale(d[1]); }) 
            .curve(d3.curveMonotoneX);
        
        svg.append("path")
            .datum(data.dataset) 
            .attr("class", "line") 
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
            .attr("d", line)
            .style("fill", "none")
            .style("stroke", "#CC0000")
            .style("stroke-width", "2");
        
        // grid
        svg.selectAll("line.horizontalGrid").data(yScale.ticks(4)).enter()
        .append("line")
            .attr({
                "class":"horizontalGrid",
                "x1" : margin.right,
                "x2" : width,
                "y1" : function(d){ return yScale(d);},
                "y2" : function(d){ return yScale(d);},
                "fill" : "none",
                "shape-rendering" : "crispEdges",
                "stroke" : "black",
                "stroke-width" : "1px"
            });
    }
});
