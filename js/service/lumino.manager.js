var lumino_manager = {
	init:function(param_obj) {

		// param_obj 내부의 속성을 확인합니다.
		if(param_obj == null) {
			console.log("!Error! / lumino_manager / init / param_obj == null");
			return;
		}

		if(param_obj.nav_container_jq == null) {
			console.log("!Error! / lumino_manager / init / param_obj.nav_container_jq == null");
			return;
		}
		this.nav_container_jq = param_obj.nav_container_jq;

		if(param_obj.sidebar_jq == null) {
			console.log("!Error! / lumino_manager / init / param_obj.sidebar_jq == null");
			return;
		}
		this.sidebar_jq = param_obj.sidebar_jq;

		this.set_event();

	}
	, set_event:function() {

		// lumino event
		// 1. log in panel
		// ul.user-menu
		var user_name_jq = this.user_name_jq = this.nav_container_jq.find("a.dropdown-toggle");
		var log_out_dropdown_jq = this.log_out_dropdown_jq = this.nav_container_jq.find("ul.dropdown-menu");

		user_name_jq.click(function(){

			if(log_out_dropdown_jq.is(":visible")) {
				log_out_dropdown_jq.hide();
			} else {
				log_out_dropdown_jq.show();
			}
		});
		log_out_dropdown_jq.click(function(){

			// lot out
			if(confirm("Log out\nAre you sure?")) {

				// expire cache
				_server.delCookie(
					// cookie name
					PROPS.PARAM_SET.COOKIE_LOGIN_FACEBOOK
				);
				_server.delCookie(
					// cookie name
					PROPS.PARAM_SET.COOKIE_LOGIN_GOOGLE
				);

				var COOKIE_LOGIN_FACEBOOK = 
				_server.getCookie(
					// cookie name
					PROPS.PARAM_SET.COOKIE_LOGIN_FACEBOOK
				);				
				var COOKIE_LOGIN_GOOGLE = 
				_server.getCookie(
					// cookie name
					PROPS.PARAM_SET.COOKIE_LOGIN_GOOGLE
				);	

				if(COOKIE_LOGIN_FACEBOOK != null) {
					// alert("COOKIE_LOGIN_FACEBOOK 실패!");
					// return;
				}

				if(COOKIE_LOGIN_GOOGLE != null) {
					// alert("COOKIE_LOGIN_GOOGLE 실패!");
					// return;
				}

				log_out_dropdown_jq.hide();

				var _param_obj = 
				_param
				.get(PROPS.PARAM_SET.IS_LOG_OUT, PROPS.PARAM_SET.YES)
				;

				// Sign out Facebook
				facebookSDK.logOut();
				
				// Sign out Google.
				googleSDK.sign_out( function() {

					_link.go_there_post(
						_link.ADMIN_LOG_IN
						, _param_obj
					);

				}, this);

			}

		});		

		// side bar collapse event
		var sidebar_jq = this.sidebar_jq;

		var sidebar_home_jq = sidebar_jq.find("li#home");
		sidebar_home_jq.click(function(){
			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}
		});

		var sidebar_quiz_jq = sidebar_jq.find("li#quiz");
		sidebar_quiz_jq.click(function(e){

			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}

			var sub_list_jq = self_jq.find("ul");
			if(sub_list_jq.is(":visible")) {
				// sub_list_jq.hide();
			} else {
				sub_list_jq.show();
			}
		});


		var sidebar_stats_jq = sidebar_jq.find("li#stats");
		sidebar_stats_jq.click(function(){
			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}

			var sub_list_jq = self_jq.find("ul");
			if(sub_list_jq.is(":visible")) {
				sub_list_jq.hide();
			} else {
				sub_list_jq.show();
			}
		});

		//quiz



		var sidebar_users_jq = sidebar_jq.find("li#users");
		sidebar_users_jq.click(function(){
			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}

			var sub_list_jq = self_jq.find("ul");
			if(sub_list_jq.is(":visible")) {
				sub_list_jq.hide();
			} else {
				sub_list_jq.show();
			}

		});


		var sidebar_log_jq = sidebar_jq.find("li#log");
		sidebar_log_jq.click(function(){
			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}

			var sub_list_jq = self_jq.find("ul");
			if(sub_list_jq.is(":visible")) {
				// sub_list_jq.hide();
			} else {
				sub_list_jq.show();
			}
		});		

		// analysis
		var sidebar_quiz_jq = sidebar_jq.find("li#analysis");
		sidebar_quiz_jq.click(function(e){

			var self_jq = $(this);
			if(self_jq.hasClass("active")) {
				return;
			}

			var sub_list_jq = self_jq.find("ul");
			if(sub_list_jq.is(":visible")) {
				// sub_list_jq.hide();
			} else {
				sub_list_jq.show();
			}
		});		

	}
}