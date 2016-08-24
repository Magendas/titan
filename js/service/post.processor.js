function on_load_gapi() {
	googleSDK.on_load_gapi();
}
$(document).ready(function(){

	// DEBUG
	$("a.navbar-brand").click(function() {
		// DEBUG MODE TOGGLE
		if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
			PROPS.IS_DEBUG_MODE = PROPS.PARAM_SET.NO;
		} else if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.NO) {
			PROPS.IS_DEBUG_MODE = PROPS.PARAM_SET.YES;
			console.log("PROPS.IS_DEBUG_MODE ::: ",PROPS.IS_DEBUG_MODE);
		}
	});

	// lumino activate
	var lumino_top_nav_bar_jq = $("nav.navbar");
	var sidebar_collapse_jq = $("div#sidebar-collapse");
	var lumino_param_obj = {
		nav_container_jq:lumino_top_nav_bar_jq
		,sidebar_jq:sidebar_collapse_jq
	}
	lumino_manager.init(lumino_param_obj);

	// facebook log in
	facebookSDK.init();	

	// SET COOKIE - REFACTOR ME
	if(_v.is_valid_str(PROPS.QUIZ_REGION)) {
		_server.setCookie(
			// cookie name
			PROPS.PARAM_SET.QUIZ_REGION
			// cookie value
			, PROPS.QUIZ_REGION
			// expire hours
			, _server.a_week_in_hours
		);
	}
	if(_v.is_valid_str(PROPS.QUIZ_LANGUAGE)) {
		_server.setCookie(
			// cookie name
			PROPS.PARAM_SET.QUIZ_LANGUAGE
			// cookie value
			, PROPS.QUIZ_LANGUAGE
			// expire hours
			, _server.a_week_in_hours
		);
	}
	if(_v.is_valid_str(PROPS.QUIZ_CATEGORY)) {
		_server.setCookie(
			// cookie name
			PROPS.PARAM_SET.QUIZ_CATEGORY
			// cookie value
			, PROPS.QUIZ_CATEGORY
			// expire hours
			, _server.a_week_in_hours
		);
	}
})
