var facebook_sdk = {
	has_initialized:false
	, init:function(fb_app_id, fb_app_ver, scope, callback) {

		if(this.has_initialized) {
			return;
		}
		this.has_initialized = true;

		window.fbAsyncInit = function() {

			console.log("HERE / facebook_sdk / window.fbAsyncInit");

			FB.init({
				appId      : fb_app_id,
				xfbml      : true,
				version    : fb_app_ver
			});

			// it calls when SDK is ready.
			if(callback != null && scope != null) {
				callback.apply(scope, []);	
			}
			
		};

		// Load the SDK asynchronously
		this.load_sdk();		

	}
	, load_sdk:function() {

		console.log("HERE / facebook_sdk / load_sdk");

	    (function(d, s, id){
		        var js, fjs = d.getElementsByTagName(s)[0];
		        if (d.getElementById(id)) {return;}
		        js = d.createElement(s); js.id = id;
		        js.src = "//connect.facebook.net/en_US/sdk.js";
		        fjs.parentNode.insertBefore(js, fjs);
			}(document, 'script', 'facebook-jssdk')
	    );
	}
	, log_in:function(scope, callback) {

		if(null == FB) {
			console.log("!Error! / null == FB");
			return;
		}

		var _self = this;

		// @ referer : https://developers.facebook.com/docs/facebook-login/web
		FB.login(function(response){

			if (response.status === 'connected') {
				// Logged into your app and Facebook.

				_self.get_user(scope, callback);
				return;

			} else if (response.status === 'not_authorized') {
				// The person is logged into Facebook, but not your app.
			} else {
				// The person is not logged into Facebook, so we're not sure if
				// they are logged into this app or not.
			}			

			var result = 
			{
				success:false
				, from:"log_in"
				, response:response
			};
			
			if(callback != null && scope != null) {
				callback.apply(scope, [result]);
			}

		}, {scope: 'public_profile,email'});

	}
	, log_out:function() {

		// 현재 로그인 상태인 것을 확인해서 로그인 되어 있다면, 로그아웃한다.
		FB.getLoginStatus(function(response) {

			if (response.status === 'connected') {

				// the user is logged in and has authenticated your
				// app, and response.authResponse supplies
				// the user's ID, a valid access token, a signed
				// request, and the time the access token 
				// and signed request each expire
				var uid = response.authResponse.userID;
				var accessToken = response.authResponse.accessToken;

				FB.logout(function(response){
					// to do something.
				});

			} else if (response.status === 'not_authorized') {

				// the user is logged in to Facebook, 
				// but has not authenticated your app

			} else {

				// the user isn't logged in to Facebook.

			}
		});		

	}
	, get_user:function(scope, callback) {

		// wonder.jung
		var fields = {"fields":"id,name,picture,email"};
		FB.api(
			'/me',
			'GET',
			fields,
			function(response) {

				console.log("response :: ",response);

				var user_id_facebook = response.id;
				var user_email_facebook = response.email;
				var user_nickname_facebook = response.name;
				var user_profile_image_facebook = response.picture.data.url;

				var callback_param = {
					success:true
					, from:"FB.api.me"
					, user_id:user_id_facebook
					, user_email:user_email_facebook
					, user_nickname:user_nickname_facebook
					, user_first_name:""
					, user_last_name:""
					, user_profile_image:user_profile_image_facebook
				}

				// Insert your code here
				callback.apply(scope, [callback_param]);
			}
		);    

	}	
}
