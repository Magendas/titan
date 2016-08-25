<?php

// common setting
include_once("../../common.inc");

$preprocessor = 
new TitanPreprocessor(
	// $mysql_interface=null
	$mysql_interface
	// $permission_arr=null / null for everyone
	, array()
);
$PROPS = $preprocessor->get_props();

// REDIRECT ON ERROR
if(	$PROPS->SUCCESS == true && !is_null($PROPS->USER_INFO) ) {
	TitanLinkManager::go(TitanLinkManager::$ADMIN_QUIZ_HOME);
}

// @ required
$mysql_interface->close();
// @ required
$feedback = $mysql_interface->get_feedback();
$PROPS->feedback = $feedback;

?>





<html>
<head>

<?php
	// @ required
	ViewRenderer::render($PROPS->HEAD_FILE_PATH,$PROPS->HEAD_VIEW_RENDER_VAR_ARR);
?>

</head>






<body role="document">

    <div class="container">

    	

		<div class="row" id="loader">
			<div class="col-xs-10 col-xs-offset-1 col-sm-8 col-sm-offset-2 col-md-4 col-md-offset-4">
				<div class="panel panel-default">
					<div class="panel-body easypiechart-panel">
						<h4>Logging on...</h4>
						<div class="loader_small" style="margin-left: 145px;margin-top: 40px;margin-bottom: 40px;"></div>
						<!-- <div class="easypiechart" id="easypiechart-blue" data-percent="92" ><span class="percent">92%</span> -->
						</div>
					</div>
				</div>
			</div>
		</div>
 	

    	<!-- Lumino Login Form begins -->
		<div class="row" id="log_in_form">
			<div class="col-xs-10 col-xs-offset-1 col-sm-8 col-sm-offset-2 col-md-4 col-md-offset-4">
				<div class="login-panel panel panel-default">
					<div 	class="panel-heading"
					<?php
						if($PROPS->IS_MOBILE) {
							echo "style=\"text-align:center;height:120px;\"";
						} else {
							echo "style=\"text-align:center;height:150px;\"";
						}
					?>
					>
					<?php
						echo "<img src=\"$PROPS->SERVICE_ROOT_PATH/images/logo/XO-logo-Alpha-B_s.png\" width=\"100%\" alt=\"...\" class=\"img-rounded\" style=\"margin-top:15px;\"><br/>";
					?>
					<span>Welcome to Quiz Simulator!</span>

					<!-- <ul class="nav nav-pills" style="padding-top:0px;"> -->
						<!-- <li role="presentation"><a id="terms_of_service" href="#"><small>Terms of service</small></a></li> -->
						<!-- <li role="presentation"><a id="privacy" href="#"><small>Privacy Policy</small></a></li> -->
					<!-- </ul> -->

					</div>
					<div class="panel-body">

						<form role="form" style="margin-bottom:0px;">

							<!-- GOOGLE LOGIN BUTTON -->
							<div class="g-signin2" data-onsuccess="on_log_in_google" style="float:left;"></div>

							<!-- FACEBOOK LOGIN BUTTON -->
							<div 	class="fb-login-button"
							<?php
								if($PROPS->IS_MOBILE) {
									echo "style=\"float:left;margin-top:15px;\"";
								} else {
									echo "style=\"float:right;\"";
								}
							?>
							data-max-rows="1"
							data-size="xlarge"
							data-show-faces="false"
							data-auto-logout-link="false" 
							scope="basic_info" 
							onlogin="on_log_in_facebook()"></div>

						</form>

						<div
						<?php
							if($PROPS->IS_MOBILE) {
								echo "style=\"margin-top:110px;\"";
							} else {
								echo "style=\"margin-top:65px;\"";
							}
						?>
						>
							<a id="terms_of_service" href="#"><small>Terms of service</small></a>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<a id="privacy" href="#"><small>Privacy Policy</small></a>
						</div>


						<!-- <ul class="nav nav-pills" style="padding-top:0px;"> -->
							<!-- <li role="presentation"><a id="terms_of_service" href="#"><small>Terms of service</small></a></li> -->
							<!-- <li role="presentation"><a id="privacy" href="#"><small>Privacy Policy</small></a></li> -->
						<!-- </ul> -->


					</div>
				</div>
			</div><!-- /.col-->
		</div><!-- /.row -->
		<!-- Lumino Login Form ends -->		

    </div> <!-- /container -->

<script>
// php to javascript sample
var PROPS = <?php echo json_encode($PROPS);?>;

// div#loader div#log_in_form
var loader_jq = $("div#loader");
loader_jq.hide();
var log_in_form_jq = $("div#log_in_form");


// log in event on facebook
var on_log_in_facebook = function() {

	loader_jq.show();
	log_in_form_jq.hide();

	var callback = 
	function(response){

		if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
			console.log("on_log_in_facebook / callback / response ::: ",response);
		}

		log_manager.write_access_log(response);
		var user_info = facebookSDK.parse_login_response(response);

		if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
			console.log("on_log_in_facebook / callback / user_info ::: ",user_info);
		}

		log_manager.write_access_log(user_info);

		log_manager.is_registered_user(
			user_info
			, _obj.get_delegate(
				// delegate_func
				function(data){

					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log("on_log_in_facebook / is_registered_user / callback / data ::: ",data);
					}

					if(data == null || data.SUCCESS == null) {

						console.log("Unexpected Error - #189.\n\nPlease report the manager.");

					} else if(!data.SUCCESS) {

						console.log("Log in failed.\n\nPlease report the manager.");

					} else if(data.USER_INFO.__status === PROPS.PARAM_SET.USER_STATUS_NOT_IN_ACTION) {
						
						console.log("USER_ID_FACEBOOK : " + data.REQ_PARAM.USER_ID_FACEBOOK + " is not valid user.\n\nPlease report the manager.");

					} else if(data.USER_INFO.__status === PROPS.PARAM_SET.USER_STATUS_AVAILABLE) {

						// var __user_id = user_info.__user_id;
						var request_param_obj = 
						_param
						.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_LOG_IN)
						.get(PROPS.PARAM_SET.USER_STATUS, PROPS.PARAM_SET.USER_STATUS_AVAILABLE)
						.get(PROPS.PARAM_SET.USER_ID_FACEBOOK, data.REQ_PARAM.USER_ID_FACEBOOK)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_PROFILE_PICTURE, user_info.FACEBOOK_USER_PROFILE_PICTURE)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_GENDER, user_info.FACEBOOK_USER_GENDER)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_AGE_RANGE, user_info.FACEBOOK_USER_AGE_RANGE)
						.get(PROPS.PARAM_SET.FACEBOOK_USER_LOCALE, user_info.FACEBOOK_USER_LOCALE)
						;	

						if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
							console.log("is_registered_user / user_info ::: ",user_info);
						}

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

									if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
										console.log("on_log_in_facebook / data ::: ",data);
									}

									if(!data.SUCCESS) {
										console.log("Unexpected Error - #236.\n\nPlease report the manager.");
										return;
									}

									log_manager.write_access_log(data);

									_server.setCookie(
										// cookie name
										PROPS.PARAM_SET.COOKIE_LOGIN_FACEBOOK
										// cookie value
										, data.REQ_PARAM.USER_ID_FACEBOOK
										// expire hours
										, _server.a_week_in_hours
									);

									var param_obj = 
									_param
									.get(PROPS.PARAM_SET.USER_ID_FACEBOOK, data.REQ_PARAM.USER_ID_FACEBOOK)
									;

									_link.go_there_post(
										_link.ADMIN_QUIZ_HOME
										, param_obj
									);

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

	var user_info = googleSDK.parse_login_response(googleUser);
	log_manager.write_access_log(user_info);

	loader_jq.show();
	log_in_form_jq.hide();

	// 메일주소가 동일하다면 같은 유저로? 
	log_manager.is_registered_user(
		user_info
		, _obj.get_delegate(
			// delegate_func
			function(data){

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("User(Google Account) on_log_in_google / is_registered_user / callback / data ::: ",data);
				}

				if(!data.SUCCESS) {

					console.log("User(Google Account) log in failed.\n\nPlease report the manager.");

				} else if(data.USER_INFO.__status === PROPS.PARAM_SET.USER_STATUS_NOT_IN_ACTION) {

					console.log("User(Google Account) status is not valid.\n\nPlease report the manager.");

				} else if(data.USER_INFO.__status === PROPS.PARAM_SET.USER_STATUS_AVAILABLE) {

					if(_v.is_not_valid_str(data.USER_INFO.__id_google)) {
						console.log("User(Google Account) is not valid user.\n\nPlease report the manager.");
						return;
					}

					_server.setCookie(
						// cookie name
						PROPS.PARAM_SET.COOKIE_LOGIN_GOOGLE
						// cookie value
						, data.USER_INFO.__id_google
						// expire hours
						, _server.a_week_in_hours
					);

					var param_obj = _param.get(PROPS.PARAM_SET.USER_ID_GOOGLE, data.USER_INFO.__id_google);

					_link.go_there_post(
						_link.ADMIN_QUIZ_HOME
						, param_obj
					);

				} else {

					console.log("User(Google Account) - unexpected Error has occured.\n\nPlease report the manager.");

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