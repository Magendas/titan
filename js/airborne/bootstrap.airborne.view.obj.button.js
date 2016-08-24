// clouser practice.
// Airbonre Collection Button Chain - refer to bootstrap.airborne.view.obj.button.js
var abc_button_chain = (function() {

	// private scope
	// variables
	var is_enable = true;
	var btn_set = {};
	var container_jq_arr = null;


	// private scope
	// functions
	function set_event(target_jq_arr, scope, callback_when_done, callback_when_event_received) {

		container_jq_arr = target_jq_arr;

		for(var idx = 0; idx < target_jq_arr.length; idx++) {
			var target_ele = target_jq_arr[idx];
			var button_jq_arr = $(target_ele).find("button");

			for(var inner_idx = 0; inner_idx < button_jq_arr.length; inner_idx++) {

				var button_ele = button_jq_arr[inner_idx];
				var button_jq = $(button_ele);
				var btn_value = button_jq.attr("btn_value");
				var btn_id = button_jq.attr("id");

				btn_set[btn_id] = button_jq;

				button_jq.off();
				button_jq.click(function(e) {

					var _self_jq = $(this);

					if(!is_enable) {
						return;
					}

					// PREPROCESS - RIGHT AFTER EVENT - INIT
					var textarea_metadata_jq = _self_jq.parent().find("textarea#meta_data");
					var meta_json_str = textarea_metadata_jq.val();

					var meta_obj = null;
					if(_v.is_valid_str(meta_json_str)) {
						meta_obj = $.parseJSON(meta_json_str);
					} else {
						meta_obj = {};	
					}
					meta_obj.value = btn_value;
					meta_obj.target_jq = _self_jq;

					// 이벤트가 진행되기 전에 사용자에게 콜백으로 진행가능한지 확인합니다.
					var is_success = false;
					if(callback_when_event_received != null) {
						is_success = callback_when_event_received.apply(scope, [meta_obj]);
					} else {
						is_success = true;
					}
					if(!is_success) {
						return;
					}
					// PREPROCESS - RIGHT AFTER EVENT - DONE

					var on_save = function(target_jq) {

						if(target_jq == null) {
							return;
						}

						var btn_value = target_jq.attr("btn_value");

						console.log("btn_value ::: ",btn_value);

						var textarea_metadata_jq = target_jq.parent().find("textarea#meta_data");
						var meta_json_str = textarea_metadata_jq.val();

						var meta_obj = null;
						if(_v.is_valid_str(meta_json_str)) {
							meta_obj = $.parseJSON(meta_json_str);
						} else {
							meta_obj = {};	
						}
						meta_obj.value = btn_value;
						meta_obj.target_jq = target_jq;
						
						if(callback_when_done != null) {
							// TODO 업데이트 관련 메타 데이터를 전달해야 한다.
							callback_when_done.apply(scope, [meta_obj]);
						}
					}					

					
					var next_bt_jq = _self_jq.next("button");

					var sibling_first_jq = null;
					if(next_bt_jq != null && 0 < next_bt_jq.length) {
						// 다음 버튼이 있습니다.
						// console.log("다음 버튼이 있습니다.");

						_self_jq.hide();						
						next_bt_jq.show();

						// call on_save
						on_save(next_bt_jq);

						return;

					} else if(next_bt_jq != null && 0 == next_bt_jq.length) {
						// 다음 버튼이 없습니다. 처음 버튼으로 돌아갑니다.
						// console.log("다음 버튼이 없습니다. 처음 버튼으로 돌아갑니다.");
						var sibling_first_jq = _self_jq.siblings("button").first();
					}

					if(sibling_first_jq != null && 0 < sibling_first_jq.length) {
						// console.log("처음 버튼을 보여줍니다.");
						_self_jq.hide();						
						sibling_first_jq.show();

						// call on_save
						on_save(sibling_first_jq);
					} else {
						console.log("!Error!");
					} // end if

				}); // end click
			} // end inner for
		} // end outer for
	}

	function _set_css_on_container(css_key, css_value) {
		container_jq_arr.css(css_key, css_value);
	}

	function _set_css_on_buttons(css_key, css_value) {
		container_jq_arr.find("button").css(css_key, css_value);
	}


	// public scope
	return {		
	    set: function(target_jq_arr, scope, callback_when_done, callback_when_event_received) {
	      set_event(target_jq_arr, scope, callback_when_done, callback_when_event_received);
	    }
	    , enable:function() {
	    	is_enable = true;
	    }
	    , disable:function() {
	    	is_enable = false;
	    }
	    , get_btn:function(id) {
	    	if(btn_set == null) {
	    		return;
	    	}
	    	return btn_set[id];
	    }
	    , show_btns:function(id, value) {
	    	
	    	var cur_showing_btn_jq = btn_set[id];
	    	var btn_jq_arr = cur_showing_btn_jq.parent().find("button");

	    	var bnt_jq_selected = null;
	    	var bnt_jq_to_show = null;
	    	for (var i = 0; i < btn_jq_arr.length; i++) {
	    		var btn_ele = btn_jq_arr[i];
	    		var btn_jq = $(btn_ele);

	    		if(btn_jq.is(":visible")) {
	    			bnt_jq_selected = btn_jq;
	    		}
	    		if(btn_jq.attr("btn_value") === value) {
	    			bnt_jq_to_show = btn_jq;
	    		}
	    	}

	    	if(bnt_jq_selected == null || bnt_jq_selected.length == 0) {
	    		return;
	    	}
	    	bnt_jq_selected.hide();

	    	if(bnt_jq_to_show == null || bnt_jq_to_show.length == 0) {
	    		return;
	    	}
	    	bnt_jq_to_show.show();

	    }
	    , get_btn_set:function() {
	    	return btn_set;
	    }
	    , set_css_on_container:function(css_key, css_value) {
	    	_set_css_on_container(css_key, css_value);
	    }
	    , set_css_on_buttons:function(css_key, css_value) {
	    	_set_css_on_buttons(css_key, css_value);	
	    }
	}

})();