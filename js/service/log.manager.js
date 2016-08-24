var log_manager = {
	// 로그인한 유저가 등록되어있는지 확인합니다.
	is_registered_user:function(user_info, callback_on_finish) {

		if(user_info == null) {
			console.log("!Error! / is_registered_user / user_info == null");
			return;
		}

		var _param_obj = 
		user_info.get(PROPS.PARAM_SET.EVENT_TYPE,PROPS.PARAM_SET.IS_SELECT_USER);

		if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
			// console.log("is_registered_user / _param_obj ::: ",_param_obj);
		}

		_ajax.post(
			// _url
			_link.get_link(_link.API_SELECT_USER)
			// _param_obj
			, _param_obj
			// _delegate_after_job_done
			, callback_on_finish
		); // ajax done.

	}
	// DB에 엑세스 로그를 등록합니다.
	,write_access_log:function(access_msg_obj) {

		if(access_msg_obj == null) {
			console.log("!Error! / write_access_log / access_msg_obj == null");
			return;
		}

		var access_msg = JSON.stringify(access_msg_obj);

		var _param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_INSERT_ACCESS_MSG)
		.get(PROPS.PARAM_SET.ACCESS_MSG, access_msg)
		;

		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_ACCESS)
			// _param_obj
			, _param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){

					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						// console.log("write_access_log / data ::: ",data);
					}
				}
				// delegate_scope
				, this
			)
		); // ajax done.

	}
	// DB에 에러 로그를 등록합니다.
	,write_error_log:function(error_msg_obj) {

		if(error_msg_obj == null) {
			console.log("!Error! / write_error_log / error_msg_obj == null");
			return;
		}


		var error_msg = JSON.stringify(error_msg_obj);
		
		var _param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE,PROPS.PARAM_SET.EVENT_TYPE_INSERT_ERROR_MSG)
		.get(PROPS.PARAM_SET.ERROR_MSG, error_msg)
		;

		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_ERROR)
			// _param_obj
			, _param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){

					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						// console.log("write_error_log / data ::: ",data);
					}
					
				}
				// delegate_scope
				, this
			)
		); // ajax done.
	}
}