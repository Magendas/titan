var googleSDK = {
	parse_login_response:function(google_user) {

		if(google_user == null) {
			console.log("!Error! / parse_login_response / google_user == null");
			return;
		}

		// Samples

		// ID: 109868663206702543271
		// log_in.php:180 Name: 정원덕
		// log_in.php:181 Image URL: https://lh5.googleusercontent.com/-lJ1FxHp66ZM/AAAAAAAAAAI/AAAAAAAAAQw/TNi2Tn0jTB4/s96-c/photo.jpg
		// log_in.php:182 Email: wonder13662@gmail.com

		var profile = google_user.getBasicProfile();

		// 1. google_user_id
		// var id_token = google_user.getAuthResponse().id_token;
		// var google_user_id = id_token;

		// 보안 이슈로 plain user id는 사용을 권장하지 않습니다만, token은 값이 지속적으로 변경되므로 id를 MD5로 해시키를 만들어 사용합니다.
		// https://developers.google.com/identity/sign-in/web/backend-auth

		// Google Developers Console
		// https://console.developers.google.com/projectselector/apis/library

		// 조건 1 : 숫자로 구성된 ip는 허용하지 않습니다.
		// http://stackoverflow.com/questions/36020374/google-permission-denied-to-generate-login-hint-for-target-domain-not-on-localh
		var google_user_id = profile.getId();

		// 2. email
		var google_user_email = profile.getEmail();
		
		// 3. first name & last name
		// 공백 포함된 이름의 처리.
		var google_user_first_name = profile.getGivenName();
		var google_user_last_name = profile.getFamilyName();

		// 4. Picture - url / Magendas로 업로드 필요.
		var google_profile_image = profile.getImageUrl();	
		var response_parsed = 
		_param
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID_TO_ENCODE_MD5,google_user_id)
		.get(PROPS.PARAM_SET.GOOGLE_USER_EMAIL,google_user_email)
		.get(PROPS.PARAM_SET.GOOGLE_USER_FIRST_NAME,google_user_first_name)
		.get(PROPS.PARAM_SET.GOOGLE_USER_LAST_NAME,google_user_last_name)
		.get(PROPS.PARAM_SET.GOOGLE_USER_PROFILE_PICTURE,google_profile_image)
		;

		return response_parsed;
	}
	, on_load_gapi:function() {

		if(gapi.auth2 != null) {
			return;
		}

		gapi.load('auth2', function() {
			gapi.auth2.init();
		});

	}
	, sign_out:function(callback, callback_scope) {

	    var auth2 = gapi.auth2.getAuthInstance();
	    auth2.signOut().then(function () {
			callback.apply(callback_scope, []);
	    });
	    
	}

}

