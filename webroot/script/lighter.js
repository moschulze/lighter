$(document).ready(function() {
    updateSceneList();
    /*window.setInterval(function() {
        updateSceneList();
    }, 5000);*/
});

var updateSceneList = function() {
    var buttonDiv = $('#buttons');
    $.ajax('/api/scene/list/')
        .done(function(result) {
            var html = '';
            $.each(result, function(key, value) {
                var cssClass = 'btn-default';
                if(value.active == true) {
                    cssClass = 'btn-primary';
                }
                html += '<div class="col-md-4"><a href="#" data-id="' + key + '" data-active="' + value.active + '" class="scene-button btn btn-lg ' + cssClass + '">' + value.name + '</a></div>';
            });
            buttonDiv.html(html);
            $('a').on('click', function(e) {
                e.preventDefault();
                if($(this).data('active') == true){
                    stopScene($(this).data('id'));
                    $(this).removeClass('btn-primary');
                    $(this).addClass('btn-default');
                    $(this).data('active', false);
                } else {
                    startScene($(this).data('id'));
                    $(this).removeClass('btn-default');
                    $(this).addClass('btn-primary');
                    $(this).data('active', true);
                }
            });
        })
        .fail(function() {
            buttonDiv.html('<p>Unable to load scene list</p>');
        });
};

var startScene = function(id) {
    $.ajax('/api/scene/start/' + id);
};

var stopScene = function(id) {
    $.ajax('/api/scene/stop/' + id);
};