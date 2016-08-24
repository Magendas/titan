$(function() {
    $('#easypiechart-teal').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });
});


$(function() {
    $('#easypiechart-orange').easyPieChart({
        scaleColor: false,
        barColor: '#ffb53e'
    });
});

$(function() {
    $('#easypiechart-red').easyPieChart({
        scaleColor: false,
        barColor: '#f9243f'
    });
});

$(function() {
   $('#easypiechart-blue').easyPieChart({
       scaleColor: false,
       barColor: '#30a5ff'
   });
});

$(function() {
    $('#easypiechart-ocean').easyPieChart({
        scaleColor: false,
        barColor: '#30a5ff'
    });
});


$(document).ready(function(){

    $('div#easypiechart-teal').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });

    $('div#easypiechart-orange').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });

    $('div#easypiechart-red').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });

    $('div#easypiechart-blue').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });

    $('div#easypiechart-ocean').easyPieChart({
        scaleColor: false,
        barColor: '#30a5ff'
    });

});

var lumino_calendar_jq = $('#calendar');
if(lumino_calendar_jq != null && 0 < lumino_calendar_jq.length) {
    $('#calendar').datepicker({});
}

