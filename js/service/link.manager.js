var link_manager = {
	init:function(root_path, airborne_server){
		this.set_root_path(root_path);
		this.set_airborne_server(airborne_server);
		this.set_param_manager(param_manager);
		return this;
	}
	,root_path:""
	,set_root_path:function(root_path){
		this.root_path = root_path;
	}
	,get_root_path:function(){
		return this.root_path;	
	}
	,airborne_server:null
	,set_airborne_server:function(airborne_server){
		this.airborne_server = airborne_server;
	}
	,get_airborne_server:function(){
		return this.airborne_server;
	}
	// @ private
	,call_url_get:function(link, param_obj){
		if(this.get_airborne_server() != null) {
			this.get_airborne_server().call_url_get(link,param_obj);
		}
	}
	// @ private
	,get_url_with_params:function(link, param_obj){
		if(this.get_airborne_server() != null) {
			return this.get_airborne_server().get_url_with_params(link, param_obj);
		}
		return link;
	}
	,go_there:function(type, param_obj){

		if(param_obj == null) {
			window.location.href = this.get_link(type);
		} else {
			this.call_url_get(this.get_link(type),param_obj);	
		}
	}
	,go_there_post:function(type, param_obj) {

		var request_url = this.get_link(type);
		if(_v.is_not_valid_str(request_url)) {
			return;
		}

		_server.post_url_with_params(
			// request_url
			request_url
			// request_param_obj
			, param_obj
		);

	}
	,refresh_post:function(param_obj) {
		this.go_there_post(this.SELF, param_obj);
	}	
	,open_new_window:function(type, param_obj){
		if(param_obj == null) {
			window.open(this.get_link(type));
		} else {
			window.open(this.get_link(type, param_obj));
		}
	}
	,get_link:function(type, param_obj){

		var link = "";

		if(type == this.SELF) {
			// 현재 페이지 주소를 가져옵니다.
			var self_url = _server.get_url_no_param();
			link = self_url;	
		} else {
			link = this.get_root_path() + type;	
		}

		if(param_obj != null) {
			return this.get_url_with_params(link, param_obj);
		}
		return link;
	}
	// PATH
	,SELF:"SELF"

	// OPEN WEB 
	,LOG_IN:"/view/log_in.php"
	,LOG_OUT:"/view/log_out.php"

	// 개인정보보호 & 이용약관
	,TOS:"/view/tos.html"
	,POS:"/view/privacy.html"

	// MANAGER
	,MANAGER_LOG_IN:"/view/manager/manager_log_in.php"
	,MANAGER_LOG_OUT:"/view/manager/manager_log_out.php"

	,MANAGER_USERS:"/view/manager/users.php"
	,MANAGER_QUIZ_HOME:"/view/manager/quiz_home.php"
	,MANAGER_QUIZ_FACTORY_LIST:"/view/manager/quiz_factory_list.php"

	,MANAGER_LOG_ACCESS:"/view/manager/log_access.php"
	,MANAGER_LOG_ERROR:"/view/manager/log_error.php"
	,MANAGER_LOG_ACTION:"/view/manager/log_action.php"

	// APIs
	,API_SELECT_USER:"/api/v1/user/select.php"
	,API_UPDATE_USER:"/api/v1/user/update.php"

	,API_UPDATE_ACCESS:"/api/v1/access/update.php"
	,API_UPDATE_ERROR:"/api/v1/error/update.php"
}