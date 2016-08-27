var kakao_sdk = {
	// @ reference : https://developers.kakao.com/docs/js-reference
	init:function(kakao_app_key) {

		if (typeof Kakao == 'undefined') {
			console.log("!Error! / undefined != Kakao");
			return;
		}

		if(_v.is_not_valid_str(kakao_app_key)) {
			console.log("!Error! / _v.is_not_valid_str(kakao_app_key)");
			return;
		}

		Kakao.init(kakao_app_key);

	} // end init
	, sign_in:function(scope, callback_on_receive_user_info) {

		// 직접 제작한 로그인/로그아웃 버튼을 사용하기 위해, sign_in/sign_out을 메서드로 제어해야 합니다.
		// 로그인 창을 띄웁니다.
		Kakao.Auth.login({
			success: function(authObj) {

				console.log("authObj :: ",authObj);

				// 아래 3가지 정보는 반드시 있어야 합니다.
				var access_token = authObj.access_token;
				var refresh_token = authObj.refresh_token;
				var scope = authObj.scope;

				// Access Token을 Javascript SDK에 설정합니다.
				Kakao.Auth.setAccessToken(access_token);

				// 유저의 프로파일 정보를 가져옵니다.
		        Kakao.API.request({
					url: '/v1/user/me',
					success: function(res) {

						console.log("Kakao.API.request / res ::: ",res);

						var user_id_kakao = res.id;
						var user_nickname_kakao = res.properties.nickname;
						// 가져온 이미지 주소를 서비스 내부적으로 사용합니다.
						var user_profile_image_kakao = res.properties.profile_image;

						var callback_param = {
							success:true
							, from:"Kakao.API.request"
							, user_id_kakao:user_id_kakao
							, user_email:""
							, user_nickname_kakao:user_nickname_kakao
							, user_profile_image_kakao:user_profile_image_kakao
						}

						if(null != callback_on_receive_user_info) {
							callback_on_receive_user_info.apply(scope, [callback_param]);	
						}

					}, // end success
					fail: function(error) {

						var callback_param = {
							success:false
							, from:"Kakao.API.request"
							, error:error
						}

						if(null != callback_on_receive_user_info) {
							callback_on_receive_user_info.apply(scope, [callback_param]);	
						}

					} // end fail
		        });			

			},
			fail: function(error) {

	          	var callback_param = {
	          		success:false
	          		, from:"Kakao.Auth.login"
	          		, error:error
	          	}

	          	if(null != callback_on_receive_user_info) {
	          		callback_on_receive_user_info.apply(scope, [callback_param]);	
	          	}

			}
		});		

	} // end sign_in
	, sign_out:function(scope, callback) {

		Kakao.Auth.logout(function() {
			callback.apply(scope, [])
		});

	} // end sign_out
}

