// clouser practice.
// Airbonre Collection Checkbox - refer to bootstrap.airborne.view.obj.checkbox.js
var abc_checkbox_input = (function() {

	// private scope
	// variables
	var _target_jq_arr;
	var _checkbox_jq_arr;
	var is_enable = true;

	// private scope
	// functions
	function set_event(target_jq_arr, callback_when_done, scope) {

		_target_jq_arr = target_jq_arr;

		// console.log("target_jq_arr ::: ",target_jq_arr);
		var checkbox_arr = [];
		for (var i = 0; i < target_jq_arr.length; i++) {
			var target_ele = target_jq_arr[i];
			var target_jq = $(target_ele);
			
			var check_box_jq = target_jq.find("input");
			checkbox_arr.push(check_box_jq);
			
		}; // end for
		_checkbox_jq_arr = checkbox_arr;

		for (var i = 0; i < checkbox_arr.length; i++) {
			var check_box_jq = checkbox_arr[i];

			check_box_jq.off();
			check_box_jq.click(function(e){

				if(!is_enable) {
					return;
				}
				
				var self_jq = $(this);
				var check_type = self_jq.attr("check_type");

				// 1. 모두 선택 / 취소한다.
				var is_checked = true;
				if(self_jq.prop('checked') !== true){
					console.log("체크 박스를 끕니다.");
				    var is_checked = false;
					self_jq.tooltip("hide");
				} else {
					console.log("체크 박스를 켭니다.");
					self_jq.tooltip("show");
				}

				if("CHECK_ALL" !== check_type) {
					return;
				}
			    for (var j = 0; j < checkbox_arr.length; j++) {
			    	var check_box_jq_to_change = checkbox_arr[j];
			    	check_box_jq_to_change.prop('checked',is_checked);
			    } // end inner for
			});
			check_box_jq.tooltip({
				html:true
				, trigger:"manual"
			});
			// check_box_jq.on('show.bs.tooltip', function () {
			check_box_jq.on('shown.bs.tooltip', function () {

			 	var self_jq = $(this);
			 	var parent_jq = self_jq.parent();

			 	var check_type = self_jq.attr("check_type");
			 	var checkbox_value = self_jq.attr("checkbox_value");

			 	var tooltip_jq = parent_jq.find("div.tooltip");
			 	tooltip_jq.click(function(e){

			 		// 삭제를 진행합니다.
			 		if(callback_when_done == null) {
			 			return;
			 		}

			 		var meta_data = {
			 			check_type:check_type
			 			, checkbox_value:checkbox_value
			 		}
			 		callback_when_done.apply(scope, [meta_data]);

			 	});
			})			
		} // end outer for
	}

	// public scope
	return {		
	    set: function(target_jq_arr, callback_when_done, scope) {

	      set_event(target_jq_arr, callback_when_done, scope);

	    }
	    , get_checked_values:function() {

	    	if(_v.is_not_valid_array(_checkbox_jq_arr)) {
	    		return;
	    	}

	    	var checked_value_arr = [];
	    	for (var i = 0; i < _checkbox_jq_arr.length; i++) {
	    		var checkbox_ele = _checkbox_jq_arr[i];
	    		var checkbox_jq = $(checkbox_ele);

	    		if(checkbox_jq.prop('checked') === false){
	    			continue;
	    		}
	    		// 켜져있는 값들만 반환.

	    		var checkbox_value = parseInt(checkbox_jq.attr("checkbox_value"));
	    		if(!(0 < checkbox_value)) {
	    			continue;
	    		}

	    		checked_value_arr.push(checkbox_value);
	    	}

	    	return checked_value_arr;
	    }
	    , enable:function() {
	    	is_enable = true;
	    }
	    , disable:function() {
	    	is_enable = false;
	    }
	    , uncheck_all:function() {
	    	// 선택된 모든 체크박스를 해제합니다.
		    for (var j = 0; j < _checkbox_jq_arr.length; j++) {
		    	var check_box_jq_to_change = _checkbox_jq_arr[j];
		    	check_box_jq_to_change.prop('checked',false);
		    } // end for
	    }	    
	}
})();