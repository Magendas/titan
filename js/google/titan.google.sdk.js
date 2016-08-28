var google_sdk = {
	callback_on_login:null
	, callback_scope_on_login:null
	, init:function(client_id, api_key) {

		if(_v.is_not_valid_str(client_id)) {
			console.log("!Error! / google_sdk.init / _v.is_not_valid_str(client_id)");
			return;
		}

		if(_v.is_not_valid_str(api_key)) {
			console.log("!Error! / google_sdk.init / _v.is_not_valid_str(api_key)");
			return;
		}

		var _self = this;
		var scopes = "profile email https://www.googleapis.com/auth/drive.readonly";

		function init_auth() {
			gapi.client.setApiKey(api_key);
			gapi.auth2.init({
				client_id: client_id
				, scope: scopes
			}).then(function () {

				// Listen for sign-in state changes.
				gapi.auth2.getAuthInstance().isSignedIn.listen(update_sign_in_status);

				// Handle the initial sign-in state.
				update_sign_in_status(gapi.auth2.getAuthInstance().isSignedIn.get());

			});
		}

		function update_sign_in_status(is_signed_in) {

			var callback_param = null;
			if (is_signed_in) {

				var google_user = gapi.auth2.getAuthInstance().currentUser.get();
				var basic_profile = google_user.getBasicProfile();

				var user_id_google = basic_profile.getId();
				var user_nickname_google = basic_profile.getName();
				var user_first_name_google = basic_profile.getGivenName();
				var user_last_name_google = basic_profile.getFamilyName();
				var user_profile_image_google = basic_profile.getImageUrl();
				var user_email_google = basic_profile.getEmail();

				// wonder.jung
				// 로그인 되었다면 위 정보로 유저 정보를 생성, 유저를 추가해줍니다.

				callback_param = {
					success:true
					, from:"gapi.auth2.getAuthInstance().signIn"
					, user_id:user_id_google
					, user_email:user_email_google
					, user_nickname:user_nickname_google
					, user_first_name:user_first_name_google
					, user_last_name:user_last_name_google
					, user_profile_image:user_profile_image_google
				}
			}

			var callback = _self.callback_on_login;
			var scope = _self.callback_scope_on_login;

			console.log("HERE / 001 / callback ::: ",callback);
			console.log("HERE / 001 / scope ::: ",scope);

			if(callback != null && scope != null && callback_param != null) {

				console.log("HERE / 002");

				// Insert your code here
				callback.apply(scope, [callback_param]);
			}
		}

		// Load the API client and auth library
		gapi.load('client:auth2', init_auth);

	}
	, log_in:function(scope, callback) {

		console.log("HERE / log_in / 001");

		if(null == gapi) {
			return;
		}

		console.log("HERE / log_in / 002");

		this.callback_on_login = callback;
		this.callback_scope_on_login = scope;

		var options = {'scope':'profile email'};
		gapi.auth2.getAuthInstance().signIn(options);

	}
	, log_out:function() {
		if(null == gapi) {
			return;
		}
		gapi.auth2.getAuthInstance().signOut();
	}

}
