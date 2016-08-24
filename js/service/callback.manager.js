// 서비스에서 반복적으로 사용하는 콜백 함수들.
var callback_manager = {
	image_input_view_on_click_quiz:function(meta_obj) {

		console.log(">>> image_input_view_on_click_quiz / meta_obj ::: ",meta_obj);

		var is_internal_image_url = false;
		if(	meta_obj != null && 
			meta_obj.quiz != null && 
			meta_obj.key === PROPS.PARAM_SET.QUIZ_IMG_LINK) {

			is_internal_image_url = 
			quiz_manager.is_internal_image_url(meta_obj.quiz.__img_link);	
		}
		if(is_internal_image_url) {
			console.log("내부 이미지인 경우는 수정을 보류. - 에이스님과 기획 회의 이후에 어떻게 할지 결정.");
			return false;
		}

		return true;		

	}
	, image_input_view_on_save_img_desc:function(meta_obj) {

		console.log(">>> image_input_view_on_save_img_desc / meta_obj ::: ",meta_obj);

		// CHECK PARAMS
		var quiz_id = parseInt(meta_obj.quiz.__id);
		if(_v.is_not_unsigned_number(quiz_id)) {
			return;
		}
		var quiz_category = meta_obj.quiz.__category;

		// 출처가 변경되었는가?
		var prev_img_desc = meta_obj.prev_img_desc;
		var next_img_desc = meta_obj.next_img_desc;
		var img_src_unchanged = (prev_img_desc === next_img_desc)?true:false;

		if(img_src_unchanged) {
			console.log("변경되지 않았다면 중단.");
			return;
		}

		// SET EVENT TYPE
		var event_type = "";
		if(PROPS.PARAM_SET.QUIZ_IMG_LINK === meta_obj.key) {
			console.log("1. image link src");
			event_type = PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG_SRC;
		} else if(PROPS.PARAM_SET.QUIZ_IMG_LINK_EXTRA === meta_obj.key) {
			console.log("2. image link extra src");
			event_type = PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG_EXTRA_SRC;
		} else {
			console.log("!Error! / _v.is_not_valid_str(event_type)");
			return;
		}		

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, event_type)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, quiz_category)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_IMG_SRC, next_img_desc)
		;
		var container_jq = meta_obj.container_jq;

		// 업데이트 이후, 변경된 정보가 attr.org에 업데이트.
		meta_obj.controller.set_prev_img_desc(next_img_desc);

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			, request_param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log(event_type + " / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #1048");
					}

					// FIX ME - 참조가 전달되지 않음.

					// 앱 중심에서 이벤트를 관리하는 객체가 있으면 편리할까?
					// 업데이트 변경!
					var btn_quiz_updated_jq = container_jq.parent().parent().find("button#quiz_updated");

					console.log(">>> btn_quiz_updated_jq ::: ",btn_quiz_updated_jq);

					if(!btn_quiz_updated_jq.is(":visible")) {
						btn_quiz_updated_jq.show();
					}

				}
				// delegate_scope
				, this
			)
		); // ajax done.		

	}
	, image_input_view_on_save_img_url:function(meta_obj) {

		// console.log(">>> image_input_view_on_save_img_url / meta_obj ::: ",meta_obj);

		// CHECK PARAMS
		var quiz_id = parseInt(meta_obj.quiz.__id);
		if(_v.is_not_unsigned_number(quiz_id)) {
			return;
		}
		var quiz_category = meta_obj.quiz.__category;
		var controller = meta_obj.controller;

		// 이미지가 변경되었는가?
		var prev_img_url = meta_obj.prev_img_url;
		var next_img_url = meta_obj.next_img_url;

		// SET EVENT TYPE
		var event_type = "";
		if(PROPS.PARAM_SET.QUIZ_IMG_LINK === meta_obj.key) {
			// console.log("1. image link");
			event_type = PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG;
		} else if(PROPS.PARAM_SET.QUIZ_IMG_LINK_EXTRA === meta_obj.key) {
			// console.log("2. image link extra");
			event_type = PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG_EXTRA;
		} else {
			console.log("!Error! / _v.is_not_valid_str(event_type)");
			return;
		}

		var is_pixabay_image_url = quiz_manager.is_pixabay_image_url(next_img_url);
		var img_unchanged = (_v.is_valid_str(next_img_url) && prev_img_url === next_img_url)?true:false;

		if(is_pixabay_image_url || img_unchanged) {
			if(PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG === event_type) {
				event_type = PROPS.PARAM_SET.EVENT_TYPE_DOWNLOAD_QUIZ_IMG_TO_SERVER;
			} else if(PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_IMG_EXTRA === event_type) {
				event_type = PROPS.PARAM_SET.EVENT_TYPE_DOWNLOAD_QUIZ_IMG_EXTRA_TO_SERVER;
			}
		}

		// 업데이트 이후, 변경된 정보가 attr.org에 업데이트.
		meta_obj.controller.set_prev_img_url(next_img_url);

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, event_type)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, quiz_category)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_IMG_LINK, next_img_url)
		;
		
		var container_jq = meta_obj.container_jq;

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			, request_param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log(event_type + " / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #1048");
					}

					// IMAGE INPUT VIEW 업데이트.
					if(data.QUERY_PARAM.EVENT_TYPE === PROPS.PARAM_SET.EVENT_TYPE_DOWNLOAD_QUIZ_IMG_TO_SERVER) {

						var __loadable_img_link = data.QUIZ_OBJ_UPDATE.__loadable_img_link;
						controller.set_prev_img_url(__loadable_img_link);

					} else if(data.QUERY_PARAM.EVENT_TYPE === PROPS.PARAM_SET.EVENT_TYPE_DOWNLOAD_QUIZ_IMG_EXTRA_TO_SERVER) {

						var __loadable_img_link_extra = data.QUIZ_OBJ_UPDATE.__loadable_img_link_extra;
						controller.set_prev_img_url(__loadable_img_link_extra);

					}

					// 업데이트 이후, 변경된 정보가 attr.org에 업데이트 되어야 함.

					// 앱 중심에서 이벤트를 관리하는 객체가 있으면 편리할까?
					// 업데이트 변경!
					var btn_quiz_updated_jq = container_jq.parent().parent().find("button#quiz_updated");
					if(!btn_quiz_updated_jq.is(":visible")) {
						btn_quiz_updated_jq.show();
					}

				},
				// delegate_scope
				this
			)
		); // ajax done.
	}
}