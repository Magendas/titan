<?php

// common setting
include_once("../common.inc");

$PROPS = 
TitanPreprocessor::get_props(
	// $mysql_interface=null
	$mysql_interface
	// $param=null
	, $param
	// $service_root_path
	, $service_root_path
	// $file_root_path
	, $file_root_path
	// $permission_arr=null
	, array()
);

if($PROPS->success == true && !is_null($PROPS->user_log_in)) {
	TitanLinkManager::go(TitanLinkManager::$ADMIN_QUIZ_HOME);
}

// @ required
$mysql_interface->close();

?>





<html>
<head>

<?php
	// @ required
	$view_port = $param->DEVICE_REPONSIVE_VIEW_PORT;
	$view_render_var_arr = 
	array(
		"[__ROOT_PATH__]"=>$service_root_path
		, "[__VIEW_PORT__]"=>$view_port
	);
	ViewRenderer::render("$file_root_path/template/head.include.template",$view_render_var_arr);	

?>

<!-- google login -->
<script src="https://apis.google.com/js/platform.js" async defer></script>
<meta name="google-signin-client_id" content="567024000478-lnj6fh33fltgv884s78ft7rlrt528pho.apps.googleusercontent.com">

</head>






<body role="document">

    <div class="container">

    	<!-- Lumino Login Form begins -->
		<div class="row">
			<div class="col-xs-10 col-xs-offset-1 col-sm-8 col-sm-offset-2 col-md-4 col-md-offset-4">
				<div class="login-panel panel panel-default">
					<div class="panel-heading" style="text-align:center;height:150px;">
					<?php
						echo "<img src=\"$service_root_path/images/logo/XO-logo-Alpha-B_s.png\" alt=\"...\" class=\"img-rounded\" style=\"margin-top:15px;\"><br/>";
					?>
					<ul class="nav nav-pills" style="padding-top:0px;">
						<li role="presentation"><a id="terms_of_service" href="#"><small>Terms of service</small></a></li>
						<li role="presentation"><a id="privacy" href="#"><small>Privacy Policy</small></a></li>
					</ul>

					</div>
					<div class="panel-body">
						<form role="form" style="margin-bottom: 0px;">

							<!-- GOOGLE LOGIN BUTTON -->
							<div class="g-signin2" data-onsuccess="on_log_in_google" style="float:left;"></div>

							<!-- FACEBOOK LOGIN BUTTON -->
							<div 	class="fb-login-button" 
									style="float:right;"
									data-max-rows="1" 
									data-size="xlarge" 
									data-show-faces="false" 
									data-auto-logout-link="false" 
									scope="basic_info" 
									onlogin="on_log_in_facebook()">
							</div>

						</form>
					</div>
				</div>
			</div><!-- /.col-->
		</div><!-- /.row -->
		<!-- Lumino Login Form ends -->	    	



    </div> <!-- /container -->

<script>
// php to javascript sample
var PROPS = <?php echo json_encode($PROPS);?>;

// log in event on facebook
var on_log_in_facebook = function() {

	console.log("페이스북으로 로그인되었습니다.");
	console.log("해당 유저의 계정정보를 가져옵니다.");

	var callback = 
	function(response){

		log_manager.write_access_log(response);

		var user_info = facebookSDK.parse_login_response(response);
		log_manager.write_access_log(user_info);

		log_manager.is_registered_user(
			user_info
			, _obj.get_delegate(
				// delegate_func
				function(data){

					var FACEBOOK_USER_ID = data.FACEBOOK_USER_ID;
					if(data.user_info.__status === PROPS.PARAM_SET.USER_STATUS_NOT_IN_ACTION) {
						
						alert("FACEBOOK_USER_ID : " + FACEBOOK_USER_ID + "(은)는 유효한 사용자가 아닙니다.\n\n관리자에게 문의해주세요.");

					} else if(data.user_info.__status === PROPS.PARAM_SET.USER_STATUS_AVAILABLE) {

						var __user_id = user_info.__user_id;
						var request_param_obj = 
						_param
						.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_LOG_IN)
						.get(PROPS.PARAM_SET.USER_STATUS, data.user_info.__status)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, FACEBOOK_USER_ID)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_PROFILE_PICTURE, user_info.FACEBOOK_USER_PROFILE_PICTURE)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_GENDER, user_info.FACEBOOK_USER_GENDER)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_AGE_RANGE, user_info.FACEBOOK_USER_AGE_RANGE)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_LOCALE, user_info.FACEBOOK_USER_LOCALE)
						;						

						var _self = this;
						_ajax.post(
							// _url
							_link.get_link(_link.API_UPDATE_USER)
							// _param_obj
							,request_param_obj
							// _delegate_after_job_done
							,_obj.get_delegate(
								// delegate_func
								function(data){

									log_manager.write_access_log(data);

									// set log in cookie
									_server.setCookie(
										// cookie name
										PROPS.PARAM_SET.COOKIELOGIN
										// cookie value
										, FACEBOOK_USER_ID
										// expire hours
										, _server.a_week_in_hours
									);

									var cookie_return = 
									_server.getCookie(
										// cookie name
										PROPS.PARAM_SET.COOKIELOGIN
									);

									if(cookie_return == null || FACEBOOK_USER_ID != cookie_return) {
										console.log("!Error! / cookie_return is not valid!");
										return;
									}
									_server.setCookie(
										// cookie name
										PROPS.PARAM_SET.COOKIE_LOGIN_FACEBOOK
										// cookie value
										, FACEBOOK_USER_ID
										// expire hours
										, _server.a_week_in_hours
									);

									if(confirm("logged in success!\nMove to quiz home?")) {

										var param_obj = _param.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, FACEBOOK_USER_ID);
										_link.go_there(
											_link.ADMIN_QUIZ_HOME
											, param_obj
										);

									}


								},
								// delegate_scope
								this
							)
						); // ajax done.

					} // end if

				}
				// delegate_scope
				, this
			)
		); // end is_registered_user function

	}

	var param_obj = {fields:PROPS.PARAM_SET.FACEBOOK_PERSONAL_INFO_XOGAMES};
	facebookSDK.getMe(
		// callback
		callback
		// callbackScope
		, this
		// param_obj
		, param_obj
	);

}

// log in event on google
var on_log_in_google = function(googleUser) {

	console.log("페이스북으로 로그인되었습니다.");
	console.log("해당 유저의 계정정보를 가져옵니다.");

	var user_info = googleSDK.parse_login_response(googleUser);
	log_manager.write_access_log(user_info);

	console.log("on_log_in_google / user_info :: ",user_info);

	// 메일주소가 동일하다면 같은 유저로? 
	log_manager.is_registered_user(
		user_info
		, _obj.get_delegate(
			// delegate_func
			function(data){

				console.log("on_log_in_google / data ::: ",data);
				
				if(data.user_info == null) {

					alert("로그인에 실패했습니다.\n\n관리자에게 문의해주세요.");

				} else if(data.user_info.__status === PROPS.PARAM_SET.USER_STATUS_NOT_IN_ACTION) {
					
					var GOOGLE_USER_ID = data.GOOGLE_USER_ID;
					alert("GOOGLE_USER_ID : " + GOOGLE_USER_ID + "(은)는 유효한 사용자가 아닙니다.\n\n관리자에게 문의해주세요.");

				} else if(data.user_info.__status === PROPS.PARAM_SET.USER_STATUS_AVAILABLE) {

					var GOOGLE_USER_HASH_KEY = data.GOOGLE_USER_HASH_KEY;
					_server.setCookie(
						// cookie name
						PROPS.PARAM_SET.COOKIE_LOGIN_GOOGLE
						// cookie value
						, GOOGLE_USER_HASH_KEY
						// expire hours
						, _server.a_week_in_hours
					);

					if(confirm("logged in success!\nMove to quiz home?")) {

						var param_obj = _param.get(PROPS.PARAM_SET.GOOGLE_USER_ID, data.GOOGLE_USER_HASH_KEY);
						_link.go_there(
							_link.ADMIN_QUIZ_HOME
							, param_obj
						);

					}

				} else {

					alert("알 수 없는 이유로 로그인에 실패했습니다.\n\n관리자에게 문의해주세요.");

				}

			}
			// delegate_scope
			, this
		)
	); // end is_registered_user function	

}





// SET EVENT ON "Terms and conditions"
// 개인정보보호 & 이용약관
var btn_terms_of_service_jq = $("a#terms_of_service");
btn_terms_of_service_jq.click(function(e){

	var modal_obj = 
	_action_modal.get(
		// title
		"Terms of service"
		// callback
		, function(modal_obj) {
			// Do nothing.
		}
		// scope
		, this
	);

	modal_obj.add_iframe(
		// title
		"Terms of service text"
		// iframe_page_link
		, _link.get_link(_link.TOS)
		// iframe_height
		, 500
		// is_scrollable
		, true
	);

	modal_obj.hide_btn_save();
	modal_obj.show();

});


var btn_privacy_jq = $("a#privacy");
btn_privacy_jq.click(function(e){

	var modal_obj = 
	_action_modal.get(
		// title
		"Privacy"
		// callback
		, function(modal_obj) {
			// Do nothing.
		}
		// scope
		, this
	);

	modal_obj.add_iframe(
		// title
		"Privacy"
		// iframe_page_link
		, _link.get_link(_link.POS)
		// iframe_height
		, 500
		// is_scrollable
		, true
	);

	modal_obj.hide_btn_save();
	modal_obj.show();

});


</script>
</body>
</html>