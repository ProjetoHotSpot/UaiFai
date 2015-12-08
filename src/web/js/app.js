(function (FB, $) {
    'use strict';

    // Scrolls to the selected menu item on the page
    $(function () {
	$('a[href*=#]:not([href=#])').click(function () {
	    if (location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') || location.hostname == this.hostname) {

		var target = $(this.hash);
		target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
		if (target.length) {
		    $('html,body').animate({
		        scrollTop: target.offset().top
		    }, 1000);
		    return false;
		}
	    }
	});
    
	    // header background
	    $.ajax({
		url: 'img/bg.jpg',
		success: function (data) {
		    $('.header').css('background-image', 'url(data:image/png;base64,' + data + ')');
		}
	    });

	    // callout bacground
	    $.ajax({
		url: 'img/callout.jpg',
		success: function (data) {
		    $('.callout').css('background-image', 'url(data:image/png;base64,' + data + ')');
		}
	    });

	    // Closes the sidebar menu
	    $("#menu-close").click(function (e) {
		e.preventDefault();
		$("#sidebar-wrapper").toggleClass("active");
	    });

	    // Opens the sidebar menu
	    $("#menu-toggle").click(function (e) {
		e.preventDefault();
		$("#sidebar-wrapper").toggleClass("active");
	    });
    });

}(window.FB, window.jQuery));
    
    function showModal(aux) {
        var title, message, footer;

        switch (aux) {
        case (1): // Login
            title = 'Erro';
            message = 'Você precisa aceitar as permissões do aplicativo para continuar. Deseja tentar novamente?';
            footer = '<button type="button" class="btn btn-default" data-dismiss="modal" onClick="fb_login()">Sim</button><button type="button" class="btn btn-default" data-dismiss="modal" onClick="showModal(4)">Não</button>';
            break;

        case (2): // Like
            title = 'Erro';
            message = '<p>Você ainda não curte nossa página, clique em "Like" e depois em continuar.<br/><iframe src="http://www.facebook.com/plugins/like.php?href=https://www.facebook.com/549489188538791" scrolling="no" frameborder="0" style="border:none; width:450px; height:80px"></iframe></p>';
            footer = '<button type="button" class="btn btn-default" data-dismiss="modal" onClick="checkLike()">Continuar</button><button type="button" class="btn btn-default" data-dismiss="modal" onClick="showModal(4)">Cancelar</button>';
            break;

        case (3): // Check-in
            title = 'Erro';
            message = 'Ocorreu um erro ao dar o Check-in. Deseja tentar novamente?';
            footer = '<button type="button" class="btn btn-default" data-dismiss="modal" onClick="userCheckin()">Sim</button><button type="button" class="btn btn-default" data-dismiss="modal" onClick="showModal(4)">Não</button>';
            break;

        case (4): // Error
            title = 'Internet bloqueada.';
            message = 'Para desbloquear, faça o Check-in.';
            footer = '</button><button type="button" class="btn btn-default" data-dismiss="modal">OK</button>';
            break;

        case (5): // Free Wifi
            title = '<h4>Internet Liberada!</h4>';
            message = 'Obrigado por fazer o Check-in! <br> Aproveite nossa internet gratuita.';
            footer = '<button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>';
            break;

        default:
            title = 'Erro!';
            message = 'Ocorreu um erro inesperado.';
            footer = '<button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>';
        }

        $('#myModal').find('.modal-header').html(title);
        $('#myModal').find('.modal-body').html(message);
        $('#myModal').find('.modal-footer').html(footer);

        $('#myModal').modal();
    }

    function userCheckin() {
        var data = {
            "message": "Hello, World! Teste de mensagem.",
            "place": "549489188538791"
        };
        FB.api('/me/feed', 'POST', data, function (response) {
            if (!response || response.error) {
                showModal(3);
            } else {
                // TODO ajax para liberar WIFI
                showModal(5);
                postRequestUserInfo();
                
            }
        });
    }

    function checkLike() {
        FB.api("me/likes/549489188538791", function (response) {
            if (response.data.length === 1) {
                userCheckin();
            } else {
                showModal(2);
            }
        });

    }

    function fbLogin() {
        FB.login(function (response) {
            if (response.authResponse) {
                checkLike();
            } else {
                showModal(1);
            }
        }, {
            scope: 'publish_actions, user_birthday, user_likes, user_location, public_profile, email'
        });
    }

    function postRequestUserInfo() {
        var data = {
            "fields": "id, name, link, email, birthday, location, gender"
        };
        FB.api('/me', 'GET', data, function (response) {

            $.ajax({
                url: '/login',
                method: "POST",
                data: 'name=' + response.name + 
                    '&link=' + response.link + 
                    '&user_id=' + response.id + 
                    '&email=' + response.email + 
                    '&birthday=' + response.birthday + 
                    '&location=' + response.location.name + 
                    '&gender=' + response.gender,
                success: function (data) {
                    showModal(5);
                    console.log(data);
                }
            });
            
        });
    }

//}(window.FB, window.jQuery));
