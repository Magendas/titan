// @ Desc : Airborne Cropper
// 이미지를 Crop해줍니다.

// closure - Airborne Collection Cropper
function get_ac_cropper(container_jq, width, height, img_url, _callback, _scope, _z_index, _img_url_error) {

	if(null == container_jq) {
		console.log("!Error! / get_cropper / container_jq == null");
		return;
	}
	if(null == _z_index || isNaN(_z_index)) {
		_z_index = 1000;
	}

	// private variables
	var canvas_tag = 
	"<canvas width=\"${width}\" height=\"${height}\" style=\"position:absolute;top: 0px;left: -1000px;padding: 0px;margin: 0px;z-index: ${z_index};\"></canvas>"
		.replace(/\$\{z_index\}/gi, _z_index)
		.replace(/\$\{width\}/gi, width)
		.replace(/\$\{height\}/gi, height)
	;
	container_jq.append(canvas_tag);
	var canvas_jq = container_jq.children().last();
	var canvas_ele = canvas_jq[0];
	var context = canvas_ele.getContext('2d');
	var cropper = {
		// public variables
		callback_on_load:_callback
		, container_jq:container_jq
		, canvas_jq:canvas_jq
		, canvas_ele:canvas_ele
		, context:context
		, width:width
		, height:height
		, img_url:img_url
		, img_url_error:_img_url_error
		, is_loaded:false
		, has_error:false
		, callback_scope:_scope
		, callback:function(result) {
		  cropper.callback_on_load.apply(cropper.scope, [result]);
		}
		// public functions
		, load:function(img_url) {
			//imageObj.src = 'http://www.html5canvastutorials.com/demos/assets/darth-vader.jpg';
			canvas_jq.css("left","-1000px");
			canvas_jq.hide();

			if(cropper.img_url != null && cropper.img_url != "") {
				image_obj.src = cropper.img_url;
			}

			cropper.is_loaded = false;
		}
		, get_img_url:function() {
			return cropper.img_url;
		}
		, get_is_loaded:function() {
			return cropper.is_loaded;
		}
		, get_has_error:function() {
			return cropper.has_error;
		}
		, is_show:function() {
			return canvas_jq.is(":visible");
		}
		, show:function() {
			canvas_jq.show();
		}
		, fade_in:function() {

			cropper.show();
			canvas_jq.css("opacity","0");
			canvas_jq.animate(
				{opacity:1}
				, 300
				, function() {
					// Animation complete.
				}
			);

		}
		, hide:function() {
			canvas_jq.hide();
		}
		, move:function(top, left) {
			canvas_jq.css("top", top+"px").css("left", left+"px");
		}
		, fade_out:function() {
			cropper.show();
			canvas_jq.css("opacity","1");
			canvas_jq.animate(
				{opacity: 0}
				, 300
				, function() {
					// Animation complete.
				}
			);
		}
		, get_img_url:function() {
			return cropper.img_url;
		}
		, is_this_you:function(img_url_to_compare) {
			if(img_url_to_compare === null || img_url_to_compare == "") {
				return false;
			}

			return (cropper.img_url === img_url_to_compare)?true:false;
		}
		, image_on_load:function() {

			// 원본의 이미지 크기를 알아야 한다.
			var src_width = parseInt(image_obj.width);
			var src_height = parseInt(image_obj.height);
			var src_x_pos = 0;
			var src_y_pos = 0;

			var dest_x_pos = 0;
			var dest_y_pos = 0;

			// 1. 원본 이미지에서 정사각형으로 크롭할 x 혹은 y 위치를 구한다.
			if(src_width < src_height) {

				// portrait 
				// width를 crop_size으로 줄임
				// height는 비율 계산이 필요

				// 1. 원본 이미지에서 정사각형으로 크롭할 y 위치를 구한다.
				src_y_pos = parseInt((src_height - src_width) / 2);

				// 정사각형으로 변경. 짧은 변을 기준으로 너비와 높이를 같게 한다. 
				src_height = src_width;

			} else if(src_height < src_width) {

				// landscape
				// height를 crop_size으로 줄임
				// width는 비율 계산이 필요

				// 1. 원본 이미지에서 정사각형으로 크롭할 x 위치를 구한다.
				src_x_pos = parseInt((src_width - src_height) / 2);

				// 정사각형으로 변경. 짧은 변을 기준으로 너비와 높이를 같게 한다. 
				src_width = src_height;

			}
			cropper.context.drawImage(
				image_obj
				, src_x_pos
				, src_y_pos
				, src_width
				, src_height
				, dest_x_pos
				, dest_y_pos
				, cropper.width
				, cropper.height
			);

			cropper.is_loaded = true;
			cropper.callback(cropper);

		}

	} // end cropper

	// Error Handling
	var image_obj = new Image();
	$(image_obj).bind('error', function (e) {

		cropper.has_error = has_error = true;

		if(_img_url_error != null && _img_url_error != "") {
			image_obj.src = _img_url_error;
		}

	});

	// private functions
	image_obj.onload = cropper.image_on_load;

	return cropper
}





























// @ Desc : Airborne Cropeller
// Cropper와 slide view를 합친 모듈. 

// 1. 여러 이미지를 한꺼번에 로딩할 수 있습니다. 
// 2. 이미지 주소를 담은 배열을 순차적으로 지정한 worker의 숫자에 맞춰 완료되는대로 queue에 다음 이미지를 로딩합니다.
// 3. 다음 경우에 대한 콜백을 제공합니다. 
// - 이미지 로딩이 늦어지는 경우. 
// - 이미지 로딩이 실패한 경우.
// - 이미지 로딩이 완료한 경우.
// 4. 이미지를 정사각형으로 크롭해줍니다.
// 5. 이미지 위에 설명을 보여줄 수 있습니다. ex) 출처등을 표기.

function get_ac_cropeller(param) {

	if(param == null) {
		console.log("!Error! / get_ac_cropeller / param == null");
		return;
	}

	// Essential Parameters
	var container_jq = param.__container_jq;
	if(container_jq == null) {
		console.log("!Error! / get_ac_cropeller / container_jq == null");
		return;
	}

	// Optional Parameters
	var top = param.__top;
	if(top == null) {
		top = 0;
	}
	var left = param.__left;
	if(left == null) {
		left = 0;
	}
	var width = param.__width;
	if(width == null) {
		width = 200;
	}
	var height = param.__height;
	if(height == null) {
		height = 200;
	}

	var z_index = parseInt(param.__z_index);

	var scope = param.__scope;
	// 로딩에 완료되었을 때의 처리
	var callback_on_loaded = param.__callback_on_loaded;
	// 로딩에 실패할 경우의 처리
	var loading_slow_threshold_sec = 2;
	var callback_on_loading_fail = param.__callback_on_loading_fail;
	// 로딩에 느릴 경우의 처리
	// 이미지 로딩의 시간을 재서 threshold seconds가 넘으면 아래 callback을 호출합니다. ex) 서버 이미지 저장등.
	var callback_on_loading_slow = param.__callback_on_loading_slow;

	var img_url_error = param.__img_url_error;

	// 한번에 로딩할 수 있는 최대 갯수.
	var cnt_on_going = 3;
	var cropper_queue_all = [];
	var cropper_queue_done = [];
	var cropper_queue_error = [];
	var cropper_queue_on_going = [];
	var cropper_queue_sleeping = [];

	// object
	var controller = {
	// public variables
	callback_on_loaded:null
	, idx_to_show_first:-1
	, img_url_arr:null
	// public functions
	, push_images:function(img_url_arr, first_image_url_to_show) {

		// VALIDATION - INIT
		if(img_url_arr == null || img_url_arr.length == 0) {
			console.log("!Error! / load_images / img_url_arr is not valid!");
			return;
		}

		controller.img_url_arr = img_url_arr;

		var idx_to_show_first = -1;
		for (var i = 0; i < img_url_arr.length; i++) {
			var img_url = img_url_arr[i];
			if(!controller.is_img_url(img_url)) {
				console.log("!Warning! / load_images / img_url is not valid! / idx :: " + i + " / url :: ",img_url);
				continue;
			}

			if(img_url === first_image_url_to_show) {
				idx_to_show_first = i;
			}

		}; // end for
		// VALIDATION - DONE

		if(idx_to_show_first != null && (-1 < idx_to_show_first)) {
			controller.idx_to_show_first = idx_to_show_first;
		}

		// 최대 10개씩 로딩. 로딩이 완료될 때마다 다음 이미지를 로딩합니다.
		controller.load_image(img_url_arr);
	}
	, get_cropper_from_queue:function(img_url) {

		if(img_url == null || img_url === "") {
			console.log("!Error! / get_cropper / img_url is not valid!");
			return;
		}

		// 이미 만든 cropper가 없는지 확인.
		// 있다면 만들어 놓은 것을 돌려줍니다.

		for (var i = 0; i < cropper_queue_all.length; i++) {

			var cropper = cropper_queue_all[i];
			if(cropper == null) {
				console.log("!Warning! / some cropper is null at idx : ",i);
				continue;
			}

			if(cropper.is_this_you(img_url)) {
				return cropper;
			}

		};

		// 이미 등록된 cropper가 없습니다. 
		// 새로 만듭니다.
		var new_cropper = 
		get_ac_cropper(
			// container_jq
			container_jq
			// width
			, width
			// height
			, height
			// img_url
			, img_url
			// callback_on_load
			, controller.callback_on_loaded_to_queue
			// scope
			, scope
			// z_index
			, z_index
			// img_url_error
			, img_url_error
		);

		// add cropper to the queue.
		controller.push_into_cropper_queue_all(new_cropper);
		if(cnt_on_going <= cropper_queue_on_going.length) {
			// console.log("로딩할 수 있는 최대 갯수가 넘는다면, 대기열에 넣습니다.");
			controller.push_into_cropper_queue_sleeping(new_cropper);
		} else {
			// console.log("로딩 큐가 비었습니다.");
			controller.push_into_cropper_queue_on_going(new_cropper);
			new_cropper.load();
		}

		return new_cropper;

	}
	, show_progress_simple:function() {

		return;

		var msg = ""
		+ "all(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_all.length)
		+ "\ndone(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_done.length)
		+ "\nerror(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_error.length)
		+ "\non going(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_on_going.length)
		+ "\nsleeping(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_sleeping.length)
		;

		console.log(msg);

	}
	, show_progress:function() {

		return;

		var msg = ""
		+ "all(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_all.length)
		+ ":" + controller.get_queue_file_names(cropper_queue_all)
		+ "\ndone(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_done.length)
		+ ":" + controller.get_queue_file_names(cropper_queue_done)
		+ "\non going(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_on_going.length)
		+ ":" + controller.get_queue_file_names(cropper_queue_on_going)
		+ "\nsleeping(${cnt})".replace(/\$\{cnt\}/gi, cropper_queue_sleeping.length)
		+ ":" + controller.get_queue_file_names(cropper_queue_sleeping)
		;

		console.log(msg);

	}
	, extract_image_name_from_url:function(img_url) {

		if(img_url == null || img_url == "") {
			return "";
		}

		return img_url.replace(/.+\/(\w+\.(png|jpeg|jpg))/gi, "$1");

	}
	, get_queue_file_names:function(cropper_queue_array) {

		var file_names = "";
		for(var idx = 0; idx < cropper_queue_array.length; idx++) {

			var cur_cropper = cropper_queue_array[idx];        
			var cur_img_link = cur_cropper.get_img_url();
			var image_file_name = cur_img_link.replace(/.+\/(\w+\.(png|jpeg|jpg))/gi, "$1");  

			if(idx == 0) {
				file_names += image_file_name;
			} else {
				file_names += "," + image_file_name;
			}
		}

		return file_names;

	}
	, hide_showing_image:function(cropper) {

	    // 이미 보여지고 이미지가 있다면 화면에서 가린다.
	    var idx_showing_image = controller.get_showing_image_idx();
	    var cropper_showing = null;
	    if(-1 < idx_showing_image) {
			cropper_showing = cropper_queue_all[idx_showing_image];
	    }
	    if(cropper_showing != null) {
			// 화면 바깥으로 이동시킴.
			cropper_showing.hide();
			cropper_showing.move(top, -1000);
	    }

	}
	, show_image:function(cropper_to_show) {

		if(cropper_to_show == null) {
			return;
		}

		// 로딩중이라면?
		if(!cropper_to_show.get_is_loaded()) {
			// console.log("로딩이 완료되지 않았습니다. / ",cropper_to_show.get_img_url());
			controller.show_progress_simple();
			return;
		}

		// TODO 사용자에게 안내 메시지와 돌돌이를?
		controller.hide_showing_image();

		// 사용자가 지정한 위치로 이동, fade in.
		cropper_to_show.move(top, left);
		cropper_to_show.fade_in();

		console.log("cropeller / show_image / ",cropper_to_show.get_img_url());

	}
	, callback_on_loaded_to_queue:function(cropper_on_loaded) {


		// Do something.
		// 1. 로딩이 완료되면 로딩 화면을 내린다. 

		var idx_to_show_first = controller.idx_to_show_first;
		if( idx_to_show_first != null && 
		  	(-1 < idx_to_show_first) && 
		  	idx_to_show_first < (controller.img_url_arr.length)) {

			var img_url_to_show_first = controller.img_url_arr[idx_to_show_first];

			if(cropper_on_loaded.is_this_you(img_url_to_show_first)) {
				// console.log("첫번째 보여줄 이미지를 화면에 띄운다.");

				// 사용자가 지정한 위치로 이동, fade in.
				cropper_on_loaded.move(top, left);
				cropper_on_loaded.fade_in();

				// 초기화
				controller.idx_to_show_first = -1;
			}
		}

		// console.log("\nImage has been loaded : ",cropper_on_loaded.get_img_url());

		// update queue
		controller.remove_from_sleeping(cropper_on_loaded);
		// controller.remove_from_queue(cropper_queue_sleeping, cropper_on_loaded);
		controller.remove_from_on_going(cropper_on_loaded);
		// controller.remove_from_queue(cropper_queue_on_going, cropper_on_loaded);

		if(cropper_on_loaded.get_has_error()) {
			controller.push_into_cropper_queue_error(cropper_on_loaded);
		} else {
			controller.push_into_cropper_queue_done(cropper_on_loaded);
		}
		
		// controller.push_into_queue(cropper_queue_all, cropper_on_loaded);

		// 작업중인 queue에 작업이 기준보다 적고, 
		// 로딩 대기중인 queue에 작업이 있다면 진행한다.
		if( 0 < cropper_queue_on_going.length && 
			cropper_queue_on_going.length < cnt_on_going && 
			0 < cropper_queue_sleeping.length ) {

			var next_cropper_to_load = cropper_queue_sleeping.shift();

			// DEBUG
			var img_name = controller.extract_image_name_from_url(next_cropper_to_load.get_img_url());
			controller.remove_from_sleeping(next_cropper_to_load);
			controller.push_into_cropper_queue_on_going(next_cropper_to_load);

			next_cropper_to_load.load();
		}

		var result = {
			cropper_queue_controller:controller
			, cropper_on_loaded:cropper_on_loaded
		};
		controller.show_progress_simple();
		callback_on_loaded.apply(scope, [result]);
	}
	, push_into_queue:function(target_queue_array, cropper_to_push) {

		if(cropper_to_push == null) {
			console.log("!Error! / push_into_queue / cropper_to_push is not valid!");
			return;
		}

		var has_already = false;
		for (var i = 0; i < target_queue_array.length; i++) {

			var cur_cropper = target_queue_array[i];
			if(cur_cropper == null) {
				console.log("!Error! / push_into_queue / cur_cropper == null");
				return;
			}

			if( cur_cropper.is_this_you(cropper_to_push.get_img_url()) ) {
				has_already = true;
			}

		}; // end for

		if(has_already) {
			console.log("!Error! / has_already");
		} else {
			target_queue_array.push(cropper_to_push);
		} // end if

		return target_queue_array;

	}
	, push_into_cropper_queue_all:function(cropper_to_push) {

		var adding_img_name = 
		controller.extract_image_name_from_url(cropper_to_push.get_img_url());
		// console.log("\npush_all / ",adding_img_name);

		if(cropper_to_push == null || cropper_to_push.get_is_loaded()) {
			console.log("!Error! / push_into_cropper_queue / cropper_to_push is not valid!");
			return;
		}
		cropper_queue_all = controller.push_into_queue(cropper_queue_all, cropper_to_push);
		controller.show_progress_simple();
	}
	, push_into_cropper_queue_sleeping:function(cropper_to_push) {

		var adding_img_name = 
		controller.extract_image_name_from_url(cropper_to_push.get_img_url());
		// console.log("\npush_sleeping / ",adding_img_name);

		if(cropper_to_push == null || cropper_to_push.get_is_loaded()) {
			console.log("!Error! / push_into_cropper_queue_sleeping / cropper_to_push is not valid!");
			return;
		}

		cropper_queue_sleeping = controller.push_into_queue(cropper_queue_sleeping, cropper_to_push);
		controller.show_progress_simple();
	}
	, push_into_cropper_queue_on_going:function(cropper_to_push) {

		var adding_img_name = 
		controller.extract_image_name_from_url(cropper_to_push.get_img_url());
		// console.log("\npush_on_going / ",adding_img_name);

		if(cropper_to_push == null) {
			console.log("!Error! / push_into_cropper_queue_on_going / cropper_to_push == null");
			return;
		}
		if(cropper_to_push.get_is_loaded()) {
			var cur_img_url = cropper_to_push.get_img_url();
			console.log("!Error! / push_into_cropper_queue_on_going / cropper_to_push.get_is_loaded() / ",cur_img_url);
			return;
		}

		cropper_queue_on_going = controller.push_into_queue(cropper_queue_on_going, cropper_to_push);
		controller.show_progress_simple();
	}
	, push_into_cropper_queue_done:function(cropper_to_push) {

		if(cropper_to_push == null || !cropper_to_push.get_is_loaded()) {
			console.log("!Error! / push_into_cropper_queue_done / cropper_to_push is not valid!");
			return;
		}

		cropper_queue_done = controller.push_into_queue(cropper_queue_done, cropper_to_push);
		controller.show_progress_simple();
	}
	, push_into_cropper_queue_error:function(cropper_to_push) {

		if(cropper_to_push == null || !cropper_to_push.get_is_loaded()) {
			console.log("!Error! / push_into_cropper_queue_error / cropper_to_push is not valid!");
			return;
		}

		cropper_queue_error = controller.push_into_queue(cropper_queue_error, cropper_to_push);
		controller.show_progress_simple();
	}
	, remove_from_queue:function(cropper_queue_array, cropper_to_remove) {

		var cropper_queue_array_next = [];
		for (var i = 0; i < cropper_queue_array.length; i++) {
			var cur_cropper = cropper_queue_array[i];
			if( cur_cropper.is_this_you(cropper_to_remove.get_img_url()) ) {
				continue;
			}
			cropper_queue_array_next.push(cur_cropper);
		}; // end for

		return cropper_queue_array_next;

	}
	, remove_from_on_going:function(cropper_to_remove) {

		var extracted_img_name = 
		controller.extract_image_name_from_url(cropper_to_remove.get_img_url());
		// console.log("remove_on_going : ",extracted_img_name);

		var cropper_queue_on_going_next = 
		controller.remove_from_queue(
			cropper_queue_on_going
			, cropper_to_remove
		);

		// CHECK
		var has_changed = false;
		if(cropper_queue_on_going_next.length < cropper_queue_on_going.length) {
			has_changed = true;
		}

		// Update queue.
		cropper_queue_on_going = cropper_queue_on_going_next;
		if(has_changed) {
			controller.show_progress_simple();  
		}
	}
	, remove_from_sleeping:function(cropper_to_remove) {

		var extracted_img_name = 
		controller.extract_image_name_from_url(cropper_to_remove.get_img_url());
		// console.log("remove_sleeping : ",extracted_img_name);

		var cropper_queue_sleeping_after_removing = 
		controller.remove_from_queue(
			cropper_queue_sleeping
			, cropper_to_remove
		);

		// CHECK
		var has_changed = false;
		if(cropper_queue_sleeping_after_removing.length < cropper_queue_sleeping.length) {
			has_changed = true;
		}

		// Update queue.
		cropper_queue_sleeping = cropper_queue_sleeping_after_removing;
		if(has_changed) {
			controller.show_progress_simple();
		}
	  
	}
	, load_image:function(img_url_arr) {

		// Priority - HIGH
		// controller.idx_to_show_first;
		var from_head = false;
		var between_head_n_tail = false;
		var from_tail = false;
		var idx_to_show_first = controller.idx_to_show_first;
		if(controller.idx_to_show_first < 1) {
			// 1. 유저가 처음 보려는 이미지가 처음 인덱스인 경우 - 순차적으로 로딩.
			from_head = true;
		} else if(0 < controller.idx_to_show_first && controller.idx_to_show_first < (img_url_arr.length - 1)){
			// 2. 유저가 처음 보려는 이미지가 처음과 마지막 사이 인덱스인 경우 - 앞뒤로 로딩.
			between_head_n_tail = true;
		} else if(controller.idx_to_show_first == (img_url_arr.length - 1)) {
			// 3. 유저가 처음 보려는 이미지가 가장 마지막 인덱스인 경우 - 역순으로 로딩.
			from_tail = true;
		}

		// Priority - MEDIUM
		// 느리게 로딩되는 것을 판단해야 함. - 서버 저장 시도. 서버 저장이 완료되면 뷰 단 정보도 업데이트. cropeller도 다시 새로운 이미지를 로딩.
		// pixabay 이미지 인것을 판단해야 함. - 서버 저장 시도. 서버 저장이 완료되면 뷰 단 정보도 업데이트. cropeller도 다시 새로운 이미지를 로딩.

		// Priority - LOW
		// 로딩이 진행중인 것을 사용자에게 알려줘야 함.

		// Error Image가 있다면 제일 먼저 로딩한다. 
		if(img_url_error != null && img_url_error != "") {
			var new_cropper = controller.get_cropper_from_queue(img_url_error);
			controller.push_into_cropper_queue_done(new_cropper);
		}

		if(from_head) {

			// console.log("처음부터 로딩합니다.");
			for(var idx = 0; idx < img_url_arr.length; idx++) {

				var img_url = img_url_arr[idx];
				if(controller.is_not_img_url(img_url)) {
					console.log("!Error! / controller.is_not_img_url(img_url)",img_url);
					return;
				}

				// 1. 해당 url의 cropper를 가져옵니다.
				var new_cropper = controller.get_cropper_from_queue(img_url);

				if(new_cropper == null || !new_cropper.get_is_loaded()) {
					// 로딩이 완료되지 않았다면 더 이상 진행하지 않습니다.
					continue;
				}

				// 로딩이 완료되었다면 cropper_queue_on_going --> cropper_queue_done로 이동시킵니다.
				controller.remove_from_sleeping(new_cropper);
				controller.remove_from_on_going(new_cropper);
				controller.push_into_cropper_queue_done(new_cropper);

			} // end outer for

		} else if(between_head_n_tail) {

			// 현재 시작 인덱스로부터 배열을 2개로 나눕니다.
			// 각 2개의 배열에서 로드할 이미지를 추가합니다.
			// console.log("중간에서 부터 로딩합니다.");
			
			var img_url_arr_head = img_url_arr.slice(0, idx_to_show_first);
			var img_url_last_idx_head = (img_url_arr_head.length - 1);

			var img_url_arr_tail = img_url_arr.slice(idx_to_show_first, img_url_arr.length);
			var img_url_last_idx_tail = (img_url_arr_tail.length - 1);

			var the_longer_length = (img_url_arr_head.length > img_url_arr_tail.length)?img_url_arr_head.length:img_url_arr_tail.length;

			for(var idx = 0; idx < the_longer_length; idx++) {

				// 앞 그룹의 이미지 주소들은 역순으로 로딩한다.
				var img_url_from_head = "";
				var img_url_idx_head = -1;
				if(idx <= img_url_last_idx_head) {
					img_url_idx_head = img_url_last_idx_head - idx;
					img_url_from_head = img_url_arr_head[img_url_idx_head];
				}
				if(img_url_from_head != "" && controller.is_not_img_url(img_url_from_head)) {

					console.log("!Error! / controller.is_not_img_url(img_url_from_head)",img_url_from_head);
					return;

				} else if(controller.is_img_url(img_url_from_head)) {

					// 1. 해당 url의 cropper를 가져옵니다.
					var new_cropper = controller.get_cropper_from_queue(img_url_from_head);
					if(new_cropper != null && new_cropper.get_is_loaded()) {
						// 로딩이 완료되었다면 cropper_queue_on_going --> cropper_queue_done로 이동시킵니다.
						controller.remove_from_sleeping(new_cropper);
						controller.remove_from_on_going(new_cropper);
						controller.push_into_cropper_queue_done(new_cropper);
					} // end if
				}

				// 뒤 그룹의 이미지 주소들은 차례대로 로딩한다.
				var img_url_from_tail = "";
				var img_url_idx_tail = -1;
				if(idx <= img_url_last_idx_tail) {
					img_url_idx_tail = idx;
					img_url_from_tail = img_url_arr_tail[img_url_idx_tail];
				}
				if(img_url_from_tail != "" && controller.is_not_img_url(img_url_from_tail)) {
					console.log("!Error! / controller.is_not_img_url(img_url_from_tail)",img_url_from_tail);
					return;

				} else if(controller.is_img_url(img_url_from_tail)) {
					// 1. 해당 url의 cropper를 가져옵니다.
					var new_cropper = controller.get_cropper_from_queue(img_url_from_tail);
					if(new_cropper != null && new_cropper.get_is_loaded()) {
						// 로딩이 완료되었다면 cropper_queue_on_going --> cropper_queue_done로 이동시킵니다.
						controller.remove_from_sleeping(new_cropper);
						controller.remove_from_on_going(new_cropper);
						controller.push_into_cropper_queue_done(new_cropper);
					} // end if

				}

			} // end outer for			

		} else if(from_tail) {

			// console.log("마지막부터 로딩합니다.");
			for(var idx = (img_url_arr.length - 1); -1 < idx; idx--) {

				var img_url = img_url_arr[idx];

				// 1. 해당 url의 cropper를 가져옵니다.
				var new_cropper = controller.get_cropper_from_queue(img_url);

				if(new_cropper == null || !new_cropper.get_is_loaded()) {
					// 로딩이 완료되지 않았다면 더 이상 진행하지 않습니다.
					continue;
				}

				// 로딩이 완료되었다면 cropper_queue_on_going --> cropper_queue_done로 이동시킵니다.
				controller.remove_from_sleeping(new_cropper);
				controller.remove_from_on_going(new_cropper);
				controller.push_into_cropper_queue_done(new_cropper);

			} // end outer for
		}

	}
	, is_not_img_url:function(target_str) {
		return !controller.is_img_url(target_str);
	}
	, is_img_url:function(target_str) {

		if(target_str == null || target_str === "") {
			return false;
		}

		// https://c1.staticflickr.com/1/168/374642314_bcc623c75b_z.jpg?zz=1

		var match_arr = target_str.match(/^(http|https).+\.(png|jpg|jpeg|gif)/gi);

		if(match_arr == null || match_arr.length == 0) {
			return false;
		}

		return true;

	}   
	, get_showing_image_url:function() {

		if(cropper_queue_all == null || cropper_queue_all.length == 0) {
			return "";
		}

		var showing_image_url = "";
		for (var i = 0; i < cropper_queue_all.length; i++) {
			var cropper = cropper_queue_all[i];
			var is_show = cropper.is_show();

			if(is_show) {
				showing_image_url = cropper.get_img_url();
				break;
			}
		};

		return showing_image_url;
	}  
	, get_showing_image_idx:function() {

		var showing_image_idx = -1;
		if(cropper_queue_all == null || cropper_queue_all.length == 0) {
			return showing_image_idx;
		}

		for (var i = 0; i < cropper_queue_all.length; i++) {
			var cropper = cropper_queue_all[i];
			var is_show = cropper.is_show();

			if(is_show) {
				showing_image_idx = i;
				break;
			}
		};

		return showing_image_idx;

	}  
	, get_cropper_with_image_url:function(image_url) {

		for (var i = 0; i < cropper_queue_done.length; i++) {;
			var cropper_done = cropper_queue_done[i];
			if(!cropper_done.get_is_loaded()) {
				continue;
			}
			var img_url_from_cropper = cropper_done.get_img_url();
			if(img_url_from_cropper === image_url) {
				return cropper_done;
			}
		};

		return null;

	}
	, show_image_with_image_url:function(image_url) {
		var cropper_to_show = controller.get_cropper_with_image_url(image_url);
		var cropper_error = null;
		if(cropper_to_show == null) {
			// 보여줄 이미지가 없다면, Error Image 노출.
			cropper_error = controller.get_cropper_with_image_url(img_url_error);
		}
		if(cropper_error != null) {
			controller.show_image(cropper_error);
		} else if(cropper_to_show == null) {
			return;
		} else {
			controller.show_image(cropper_to_show);
		}
	}
	, hide_all:function() {

		for (var i = 0; i < cropper_queue_done.length; i++) {;
			var cropper_done = cropper_queue_done[i];
			if(!cropper_done.get_is_loaded()) {
				continue;
			}
			cropper_done.hide();
		};

	}


	} // end cropper

	return controller;

}

// @ Usage

/*
// 100개의 이미지를 미리 불러온다.
var img_url_arr = [
  "https://upload.wikimedia.org/wikipedia/commons/thumb/3/30/Mercury_in_color_-_Prockter07_centered.jpg/1033px-Mercury_in_color_-_Prockter07_centered.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/08/10/08/17/dog-414570_960_720.jpg"
  ,"https://i.ytimg.com/vi/2-Hmbh5ONYk/maxresdefault.jpg"
  ,"https://c1.staticflickr.com/9/8630/16107473929_6d534aaac7_b.jpg"
  ,"https://i.ytimg.com/vi/CC5iKbMRvhc/maxresdefault.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/7/74/Jack_Black_2_2011.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/e/e7/QueenPerforming1977.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/1/1f/Jean-Fran%C3%A7ois_Millet_-_Gleaners_-_Google_Art_Project_2.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/4/47/Piet_Mondriaan_-_03.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/04/13/17/21/salvador-dali-720882_960_720.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/thumb/0/09/Pope_Francis_Korea_Haemi_Castle_19_(cropped).jpg/811px-Pope_Francis_Korea_Haemi_Castle_19_(cropped).jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/e/e3/Kheops-Pyramid.jpg"
  ,"https://c2.staticflickr.com/8/7241/7331211014_331f9f76e6_b.jpg"
  ,"https://c2.staticflickr.com/4/3838/14217863550_cfae690f21_b.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/thumb/d/d5/David_von_Michelangelo.jpg/402px-David_von_Michelangelo.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/9/9e/Possible_Self-Portrait_of_Leonardo_da_Vinci.jpg"
  ,"http://farm6.static.flickr.com/5089/5361925402_402180795c.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/05/03/12/59/painting-337049_960_720.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/08/15/15/13/bordeaux-889619_960_720.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/10/03/19/58/gollum-472144_960_720.jpg"
  ,"http://img09.deviantart.net/a77f/i/2015/090/3/b/thranduil_by_pelegrin_tn-d8h71mt.jpg"
  ,"https://i.ytimg.com/vi/gZ7tPVZ9640/maxresdefault.jpg"
  ,"https://c2.staticflickr.com/4/3141/2683374643_8a6b1be995.jpg"
  ,"http://farm4.static.flickr.com/3759/13628118464_c2f8836771.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/5/5e/Audrey_Hepburn_1956.jpg"
  ,"https://farm8.staticflickr.com/7613/17097381065_6589fcdffc_o_d.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/7/7f/Daniel_Craig_-_Film_Premiere_%22Spectre%22_007_-_on_the_Red_Carpet_in_Berlin_(22387409720)_(cropped).jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/1/1c/Pierce_Brosnan_Berlinale_2014_-_02.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/0/07/Marlon_Brando.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/7/73/Niccolo_Paganini01.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/a/ac/Bach_face.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/e/ef/Hugh_Jackman_2015.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/4/49/Daniel_Henney_cropped.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/0/02/Anne_Hathaway_2008.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/a/ad/Not-AI.gif"
  ,"http://orig01.deviantart.net/a27a/f/2015/106/6/8/hades_sketch_by_tana_jo-d8pwaov.png"
  ,"https://upload.wikimedia.org/wikipedia/commons/f/fe/Liam_Neeson_Deauville_2012_2.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/7/71/Tom_Cruise_avp_2014_4.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/0/0d/Jackie_Chan_TIFF_2005.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/0/09/Jackie_Chan_Cannes.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/c/ca/Bruce_Lee_1973.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/3/3d/UN_Secretary-General_Ban_Ki-moon_-_Flickr_-_The_Official_CTBTO_Photostream_(13).jpg"
  ,"https://pixabay.com/static/uploads/photo/2013/12/11/01/29/peoples-republic-of-china-226709_960_720.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/d/d0/Sciurus_vulgaris_bearn_2004.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/07/25/06/58/albert-einstein-401484_960_720.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/0/0f/Luke_Evans_2014_(cropped).jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/c/c0/Antonio_del_Pollaiolo_-_Ercole_e_l'Idra_e_Ercole_e_Anteo_-_Google_Art_Project.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/05/28/08/47/taj-mahal-787725_960_720.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/04/26/10/15/sagrada-familia-332392_960_720.jpg"
  ,"https://c2.staticflickr.com/8/7334/9269038144_78649a9bca.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/4/4b/Giordano's_Deep_Dish_Pizza.jpg"
  ,"https://pixabay.com/static/uploads/photo/2014/10/12/20/34/disneyland-486098_960_720.jpg"
  ,"https://c2.staticflickr.com/6/5255/5398192567_21c41699e4_b.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/07/03/20/06/orang-utan-830661_960_720.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/6/64/Confit_byaldi_1.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/09/13/15/27/peony-938282_960_720.jpg"
  ,"https://upload.wikimedia.org/wikipedia/commons/1/1d/Hylocereus_undatus_red_pitahaya.jpg"
  ,"http://hdfreewallpaper.net/wp-content/uploads/2015/08/25-beautiful-sunrise-free-hd-wallpapers.jpg"
  ,"https://pixabay.com/static/uploads/photo/2015/11/16/18/20/peanuts-1046140_960_720.jpg"
  ,"http://hdfreewallpaper.net/wp-content/uploads/2015/08/25-beautiful-sunrise-free-hd-wallpapers.jpg"
  ,"http://static.trustedreviews.com/94/00003790c/5bad_orh370w630/296e079a-568e-4c37-bb12-bb1cc731e0ab-34ZgP.jpg"
];

var test_arr = img_url_arr.slice(0,10);
var container_jq = $("div#container");
var cropper_queue_obj = 
get_cropper_queue(
	{
		__container_jq:container_jq
		, __top:100
		, __left:100
		, __width:100
		, __height:100
		, __scope:this
		, __callback_on_loaded:function(data){

			console.log("callback_on_loaded / data ::: ");

		}
		, __callback_on_loading_fail:function(data){

			console.log("callback_on_loading_fail / data ::: ");

		}
		, __callback_on_loading_slow:function(data){

			console.log("callback_on_loading_slow / data ::: ");

		}
	}
);
cropper_queue_obj.push_images(test_arr, 0);

$( document ).keyup(function(e) {

	var showing_image_url = cropper_queue_obj.get_showing_image_url();
	var selected_idx = -1;
	for(var i=0; i < test_arr.length;i++) {
		var img_url = test_arr[i];
		if(showing_image_url == img_url) {
			selected_idx = i;
		}
	}
	if(selected_idx < 0) {
		return;
	}

	console.log("Press Left / showing_image_url :: ",showing_image_url);
	console.log("selected_idx ::: ",selected_idx);

	// 다음 보여줘야할 img url을 가져온다. 
	var next_img_url = "";
	if(e.which == 37) { // 
		selected_idx--;
	} else if(e.which == 39) {
		selected_idx++;
	}
	if(selected_idx < 0 || (test_arr.length - 1) < selected_idx) {
		return;
	}

	// 가져온 img url와 매칭되는 cropper를 화면에 띄운다.
	next_img_url = test_arr[selected_idx];
	cropper_queue_obj.show_image_with_image_url(next_img_url);

})
*/




