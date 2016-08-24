// clouser practice.
// Airbonre Collection View Input - refer to bootstrap.airborne.view.obj.input.js
var abc_view_input = (function() {

	// private scope
	// variables
	var is_enable = true;

	// private scope
	// functions
	function set_event(target_jq_arr, scope, callback_when_done, callback_before_show, callback_str_cnt) {

		target_jq_arr.click(function(e){

			if(!is_enable) {
				return;
			}

			var container_jq = _self_jq = $(this);

			var text_on_view_jq = _self_jq.find("span#text_on_view");

			var view_text_jq = text_on_view_jq.find("small#text");
			var textarea_storage_jq = text_on_view_jq.find("textarea");

			var text_on_input_jq = _self_jq.find("span#text_on_input");

			var btn_str_len_on_input_jq = text_on_input_jq.find("button#str_len");
			var input_jq = text_on_input_jq.find("input");
			
			var text_src = textarea_storage_jq.text();
			input_jq.val(text_src);

			var btn_save_jq = text_on_input_jq.find("button#btn_save");
			var btn_str_len_on_view_jq = text_on_view_jq.find("button#str_len");

			if(!text_on_view_jq.is(':visible')) {
				return;
			}
			
			// VIEW UPDATE
			text_on_view_jq.hide();
			text_on_input_jq.show();

			if(!input_jq.is(":focus")) {
				input_jq.focus();	
			}

			var max_char = parseInt(input_jq.attr("max_char"));
			var cur_user_input = input_jq.val();
			var cur_user_input_length = cur_user_input.length;

			// 사용자가 지정한 방식으로 문자열을 셀 경우의 처리.
			var custom_input_length = -1;
			if(callback_str_cnt != null) {
				custom_input_length = callback_str_cnt.apply(scope, [cur_user_input]);
			}
			if(-1 < custom_input_length) {
				cur_user_input_length = custom_input_length;
			}

			var char_cnt_left = max_char - cur_user_input_length;

			btn_str_len_on_input_jq.html(char_cnt_left);

			var on_save = function(target_jq) {

				if(target_jq == null) {
					return;
				}

				var unique_id = target_jq.attr("id");

				var text_on_view_jq = target_jq.find("span#text_on_view");
				var textarea_storage_jq = text_on_view_jq.find("textarea");

				var text_on_input_jq = target_jq.find("span#text_on_input");

				var btn_str_len_on_input_jq = text_on_input_jq.find("button#str_len");
				var input_jq = text_on_input_jq.find("input");
				var btn_save_jq = text_on_input_jq.find("button#btn_save");

				var btn_str_len_on_view_jq = text_on_view_jq.find("button#str_len");

				var textarea_metadata_jq = target_jq.find("textarea#meta_data");
				var meta_json_str = textarea_metadata_jq.val();

				// REMOVE ME
				// 글자수가 넘으면 중단 - 넘어도 입력하는 것으로 변경
				// var is_warning_sign = btn_str_len_on_input_jq.hasClass("btn-danger");
				// if(is_warning_sign) {
				// 	return;
				// }
				var cur_user_input = input_jq.val();

				// view text update!
				if(callback_before_show != null) {
					cur_user_input = callback_before_show.apply(scope, [cur_user_input]);
				}

				var view_text_jq = text_on_view_jq.find("small#text");
				view_text_jq.html(cur_user_input);

				// view text cnt update!
				var cur_user_input_length = cur_user_input.length;
				// 사용자가 지정한 방식으로 문자열을 셀 경우의 처리.
				var custom_input_length = -1;
				if(callback_str_cnt != null) {
					custom_input_length = callback_str_cnt.apply(scope, [cur_user_input]);
				}
				if(-1 < custom_input_length) {
					cur_user_input_length = custom_input_length;
				}

				btn_str_len_on_view_jq.html(cur_user_input_length);

				var max_char = parseInt(input_jq.attr("max_char"));
				if(max_char < cur_user_input_length) {
					btn_str_len_on_input_jq.removeClass("btn-default");
					btn_str_len_on_input_jq.addClass("btn-danger");

					btn_str_len_on_view_jq.removeClass("btn-default");
					btn_str_len_on_view_jq.addClass("btn-danger");
				} else {
					btn_str_len_on_input_jq.removeClass("btn-danger");
					btn_str_len_on_input_jq.addClass("btn-default");

					btn_str_len_on_view_jq.removeClass("btn-danger");
					btn_str_len_on_view_jq.addClass("btn-default");
				}

				// view update
				text_on_view_jq.show();
				text_on_input_jq.hide();

				// remove event
				$( document ).unbind( "keyup" );

				// remove interval
				clearInterval(maxchar_interval);

				// ajax update!
				// EVENT CALLBACK - ajax update - 업데이트가 성공하면 아래 절차를 진행한다.
				var meta_obj = null;
				if(_v.is_valid_str(meta_json_str)) {
					meta_obj = $.parseJSON(meta_json_str);
				} else {
					meta_obj = {};
				}
				meta_obj.value = cur_user_input;
				meta_obj.view_text_jq = view_text_jq;
				meta_obj.container_jq = container_jq;

				// textarea storage update!
				textarea_storage_jq.html(cur_user_input);

				if(callback_when_done != null) {
					// TODO 업데이트 관련 메타 데이터를 전달해야 한다.
					callback_when_done.apply(scope, [meta_obj]);
				}
			}

			// BTN EVENT
			btn_save_jq.off();
			btn_save_jq.click(function(e){

				e.stopPropagation();
				e.preventDefault();

				on_save(_self_jq);

			});

			// $( document ).keyup(function( event ) {
			input_jq.keyup(function( event ) {

				if(event.which == 13) {
					// ENTER KEY
					on_save(_self_jq);
				} else if(event.which == 27) {
					// ESCAPE - CANCEL
					// view update
					text_on_view_jq.show();
					text_on_input_jq.hide();

					// remove event
					// $( document ).unbind( "keyup" );
					$(this).unbind( "keyup" );
				}
			});

			// TEXT CHANGE DETECTION
			var char_cnt_left_prev = char_cnt_left;
			var maxchar_interval = setInterval(function(){

				var cur_user_input = input_jq.val();
				var max_char = parseInt(input_jq.attr("max_char"));

				var cur_user_input_length = cur_user_input.length;
				var char_cnt_left = max_char - cur_user_input_length;

				// 사용자가 지정한 방식으로 문자열을 셀 경우의 처리.
				var custom_input_length = -1;
				if(callback_str_cnt != null) {
					custom_input_length = callback_str_cnt.apply(scope, [cur_user_input]);
				}
				if(-1 < custom_input_length) {
					char_cnt_left = max_char - custom_input_length;
				}

				if(char_cnt_left_prev == char_cnt_left) {
					return;
				}
				char_cnt_left_prev = char_cnt_left;

				btn_str_len_on_input_jq.html(char_cnt_left);

				is_warning_sign = btn_str_len_on_input_jq.hasClass("btn-danger");
				if(!is_warning_sign && char_cnt_left < 0) {

					btn_str_len_on_input_jq.removeClass("btn-default");
					btn_str_len_on_input_jq.addClass("btn-danger");

					// REMOVE ME
					// btn_save_jq.prop("disabled", true);

				} else if(is_warning_sign && -1 < char_cnt_left){

					btn_str_len_on_input_jq.removeClass("btn-danger");
					btn_str_len_on_input_jq.addClass("btn-default");

					// REMOVE ME
					// btn_save_jq.prop("disabled", false);

				}

			//}, 50); // end interval
//	TEST
			}, 500); // end interval
		}); // end click
		
	}

	// public scope
	return {		
	    set: function(target_jq_arr, scope, callback_when_done, callback_before_show, callback_str_cnt) {
	      set_event(target_jq_arr, scope, callback_when_done, callback_before_show, callback_str_cnt);
	    }
	    , enable:function() {
	    	is_enable = true;
	    }
	    , disable:function() {
	    	is_enable = false;
	    }		    
	}

})();






// image input
var abc_image_input = (function() {

	// private scope
	// variables
	var is_enable = true;

	// private scope
	// functions
	function set_event(target_jq_arr, scope, callback_on_save_img_url, callback_on_save_img_desc, callback_when_event_received) {

		var on_save_image_url = function(target_jq) {

			var self_jq = target_jq;
			var super_parent_jq = self_jq.parent().parent().parent().parent();

			var image_info_group_jq = super_parent_jq.find("span#image_info_group");
			var image_on_view = super_parent_jq.find("span#image_on_view");
			var image_view_jq = image_on_view.find("img");

			var input_img_url_jq = image_info_group_jq.find("div#input_img_url input");
			input_img_url_jq.unbind( "keyup" );

			var prev_img_url = input_img_url_jq.attr("org");
			var next_img_url = input_img_url_jq.val();

			image_info_group_jq.hide();

			// UPDATE IMAGE URL ON IMAGE VIEW
			if(prev_img_url !== next_img_url) {
				image_view_jq.attr("src",next_img_url);
			}

			var textarea_metadata_jq = super_parent_jq.find("textarea#meta_data");
			var meta_json_str = textarea_metadata_jq.val();

			var meta_obj = null;
			if(_v.is_valid_str(meta_json_str)) {
				meta_obj = $.parseJSON(meta_json_str);
			} else {
				meta_obj = {};
			}
			meta_obj.value = next_img_url;

			meta_obj.prev_img_url = prev_img_url;
			meta_obj.next_img_url = next_img_url;

			meta_obj.container_jq = super_parent_jq;
			meta_obj.controller = {
				image_view_jq:image_view_jq
				,input_img_url_jq:input_img_url_jq
				, set_prev_img_url:function(new_img_url) {
					this.image_view_jq.attr("src",new_img_url);
					this.input_img_url_jq.attr("org",new_img_url);
				}
			};

			// UPDATE IMAGE TO DB - CALLBACK
			if(callback_on_save_img_url != null) {
				// TODO 업데이트 관련 메타 데이터를 전달해야 한다.
				callback_on_save_img_url.apply(scope, [meta_obj]);
			}

		}

		var on_save_image_desc = function(target_jq) {

			var self_jq = target_jq;
			var super_parent_jq = self_jq.parent().parent().parent().parent();

			var image_info_group_jq = super_parent_jq.find("span#image_info_group");
			var image_on_view = super_parent_jq.find("span#image_on_view");
			var image_desc_jq = image_on_view.find("span#image_desc");

			var input_img_desc_jq = image_info_group_jq.find("div#input_img_desc input");
			input_img_desc_jq.unbind( "keyup" );

			var prev_img_desc = input_img_desc_jq.attr("org");
			var next_img_desc = input_img_desc_jq.val();
			if(prev_img_desc !== next_img_desc) {
				image_desc_jq.html(next_img_desc);
			}

			var textarea_metadata_jq = super_parent_jq.find("textarea#meta_data");
			var meta_json_str = textarea_metadata_jq.val();

			var meta_obj = null;
			if(_v.is_valid_str(meta_json_str)) {
				meta_obj = $.parseJSON(meta_json_str);
			} else {
				meta_obj = {};
			}
			meta_obj.prev_img_desc = prev_img_desc;
			meta_obj.next_img_desc = next_img_desc;
			meta_obj.container_jq = super_parent_jq;

			meta_obj.controller = {
				input_img_desc_jq:input_img_desc_jq
				, set_prev_img_desc:function(new_img_url) {
					this.input_img_desc_jq.attr("org",next_img_desc);
				}
			};


			// UPDATE IMAGE TO DB - CALLBACK
			if(callback_on_save_img_desc != null) {
				// TODO 업데이트 관련 메타 데이터를 전달해야 한다.
				callback_on_save_img_desc.apply(scope, [meta_obj]);
			}

		}		

		for (var i = 0; i < target_jq_arr.length; i++) {
			var target_ele = target_jq_arr[i];
			var target_jq = $(target_ele);

			target_jq.off();
			target_jq.click(function(e){

				e.stopPropagation();
				e.preventDefault();

				var self_jq = $(this);
				var image_info_group_jq = self_jq.find("span#image_info_group");

				// 이미지 정보 버튼과 입력창
				var btn_save_img_desc_jq = image_info_group_jq.find("button#btn_save_img_desc"); // diput
				var input_img_desc_jq = image_info_group_jq.find("div#input_img_desc input");

				// 이미지 주소 버튼과 입력창
				var btn_save_img_url_jq = image_info_group_jq.find("button#btn_save_img_url"); // diput
				var input_img_url_jq = image_info_group_jq.find("div#input_img_url input");


				var super_parent_jq = btn_save_img_url_jq.parent().parent().parent().parent();
				var textarea_metadata_jq = super_parent_jq.find("textarea#meta_data");
				var meta_json_str = textarea_metadata_jq.val();
				
				var image_view_jq = super_parent_jq.find("img");

				var meta_obj = null;
				if(_v.is_valid_str(meta_json_str)) {
					meta_obj = $.parseJSON(meta_json_str);
				} else {
					meta_obj = {};
				}
				meta_obj.value = image_info_group_jq.val();
				meta_obj.container_jq = super_parent_jq;
				meta_obj.controller = {
					image_view_jq:image_view_jq
					, set_img_url:function(new_img_url) {
						this.image_view_jq.attr("src",new_img_url);
					}
				};

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
				if(!is_enable) {
					return;
				}

				// 입력 모음 창을 보여줍니다.
				image_info_group_jq.show();
				
				// SET EVENT
				// 이미지 정보 버튼과 입력창 이벤트
				btn_save_img_desc_jq.off();
				btn_save_img_desc_jq.click(function(e){

					e.stopPropagation();
					e.preventDefault();

					var _self_jq = $(this);
					
					_self_jq.off();
					image_info_group_jq.off();
					image_info_group_jq.unbind( "keyup" );

					on_save_image_desc(_self_jq);
				});

				console.log("input_img_desc_jq ::: ",input_img_desc_jq);

				input_img_desc_jq.off();
				input_img_desc_jq.unbind( "keyup" );
				input_img_desc_jq.keyup(function( e ) {

					console.log("input_img_desc_jq.keyup");

					e.stopPropagation();
					e.preventDefault();

					var _self_jq = $(this);

					if(e.which == 13) {
						// ENTER KEY
						image_info_group_jq.hide();
						_self_jq.unbind( "keyup" );
						btn_save_img_desc_jq.off();
						on_save_image_desc(btn_save_img_desc_jq);
					} else if(e.which == 27) {
						image_info_group_jq.hide();
						_self_jq.unbind( "keyup" );
						btn_save_img_desc_jq.off();
					}

				}); // end key up

				// 이미지 주소 버튼과 입력창 이벤트
				btn_save_img_url_jq.off();
				btn_save_img_url_jq.click(function(e){

					e.stopPropagation();
					e.preventDefault();

					var _self_jq = $(this);
					
					_self_jq.off();
					image_info_group_jq.off();
					image_info_group_jq.unbind( "keyup" );

					on_save_image_url(_self_jq);
				});

				input_img_url_jq.off();
				input_img_url_jq.unbind( "keyup" );
				input_img_url_jq.keyup(function( e ) {

					e.stopPropagation();
					e.preventDefault();

					var _self_jq = $(this);

					if(e.which == 13) {
						// ENTER KEY
						image_info_group_jq.hide();
						_self_jq.unbind( "keyup" );
						btn_save_img_url_jq.off();
						on_save_image_url(btn_save_img_url_jq);
					} else if(e.which == 27) {
						image_info_group_jq.hide();
						_self_jq.unbind( "keyup" );
						btn_save_img_url_jq.off();
					}

				}); // end key up
			});			
		};

	}
	// public scope
	return {		
	    set: function(target_jq_arr, scope, callback_on_save_img_url, callback_on_save_img_desc, callback_when_event_received) {
	      set_event(target_jq_arr, scope, callback_on_save_img_url, callback_on_save_img_desc, callback_when_event_received);
	    }
	    , enable:function() {
	    	is_enable = true;
	    }
	    , disable:function() {
	    	is_enable = false;
	    }		    

	}

})();

