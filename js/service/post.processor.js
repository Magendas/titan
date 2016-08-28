function on_load_gapi() {
	googleSDK.on_load_gapi();
}
function login_with_kakao() {


	kakao_sdk.log_in(
		// scope
		this
		// callback_on_receive_user_info
		, function(result) {

			console.log("callback_on_receive_user_info  / result ::: ",result);

			// 카카오톡 로그인 시에 유저 정보는 닉네임과 프로필 이미지 주소만 얻을수 있습니다.
			// 이메일, 연락처 정보는 가입 유저 추가 정보로 입력해야 합니다.

			// TODO sign up manager를 만들어야 할듯. 
			// 1. 신규 가입
			// 2. 기존 회원 로그인
			// 3. 탈퇴 

			// 위 기능들을 관리하는 javascript 모듈.

		}
	);
}
function login_with_facebook() {

	facebook_sdk.log_in(this, function(response){
		console.log("login_with_facebook / response ::: ",response);
	});

}
function login_with_google() {

	google_sdk.log_in(this, function(response){
		console.log("login_with_google / response ::: ",response);
	});

}
function log_out() {
	kakao_sdk.log_out();
	facebook_sdk.log_out();
	google_sdk.log_out();
}
$(document).ready(function(){

	// DEBUG
	console.log("PROPS.IS_DEBUG_MODE ::: ",PROPS.IS_DEBUG_MODE);

	// 운영툴일 경우만 lumino가 동작함.
	// http://stackoverflow.com/questions/858181/how-to-check-a-not-defined-variable-in-javascript
	if (typeof lumino_manager != 'undefined') {
		// lumino activate
		var lumino_top_nav_bar_jq = $("nav.navbar");
		var sidebar_collapse_jq = $("div#sidebar-collapse");
		var lumino_param_obj = {
			nav_container_jq:lumino_top_nav_bar_jq
			,sidebar_jq:sidebar_collapse_jq
		}
		lumino_manager.init(lumino_param_obj);
	}

	// kakao auth init
	var kakao_app_key = PROPS.DEV_PROPS.SERVICE_CONST.APP_KAKAO.KEY_JAVASCRIPT;
	kakao_sdk.init(kakao_app_key);

	// google auth init
	var google_client_id = PROPS.DEV_PROPS.SERVICE_CONST.APP_GOOGLE.CLIENT_ID;
	var google_api_key = PROPS.DEV_PROPS.SERVICE_CONST.APP_GOOGLE.API_KEY;

	google_sdk.init(
		// client_id
		google_client_id
		// api_key
		, google_api_key
	);

	// facebook auth init
	var fb_app_id = PROPS.DEV_PROPS.SERVICE_CONST.APP_FACEBOOK.ID;
	var fb_app_ver = PROPS.DEV_PROPS.SERVICE_CONST.APP_FACEBOOK.VERSION;

	facebook_sdk.init(
		// fb_app_id
		fb_app_id
		// fb_app_ver
		, fb_app_ver
		// scope
		, this
		// callback
		, function() {
			// Do something...
		}
	);	// end init


})
