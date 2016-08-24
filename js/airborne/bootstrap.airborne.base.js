// airborne for bootstrap - 2014

var airborne = {
	validator:null
	,server:null	
	,bootstrap:{
		obj:{
		}
		,column:null
		,modal:null
		,table_manager:null
		,view:{
			obj:{
				list:null
				,table:null
				,__action:null
				,__action_list:null
				,__action_table:null
			}
			,mobile:{
				list:null
			}
		}
	}
	,ajax:null
};

airborne.console = {
	/*
		@ public
		@ scope : console
		@ desc : 콘솔 오브젝트를 반환합니다. 
		특정 scope에서만 콘솔 로그 노출 여부를 on/off 하기 위해 이 객체를 사용합니다.
	*/
	get:function(scope){
		var consoler = {
			scope:scope
			,setScope:function(scope){
				this.scope = scope;
			}
			,say:function(msg, obj){

				var _v = airborne.validator;
				var _obj = airborne.bootstrap.obj;

				if(this.isShow == false) return;

				if(this.scope != undefined && this.scope != "") {
					if(obj != undefined) {
						console.log(this.scope + " / " + msg,obj);
					} else if(msg === "") {
						console.log("");
					} else {
						console.log(this.scope + " / " + msg);
					}
				} else {
					if(obj != undefined) {
						console.log(msg,obj);
					} else {
						console.log(msg);
					}
				}
			}
			,say_err:function(msg, obj) {
				this.say("!Error! / " + msg, obj);
			}
			,isShow:true
			,on:function(){
				this.isShow=true;
			}
			,off:function(){
				this.isShow=false;
			}
		};

		return consoler;
	}
}


airborne.client = {
	get_os:function() {
		return navigator.platform;
	}
	, get_browser:function() {
		return navigator.appName;
	}
	, get_ip:function() {
		return;	
	}
}

//airborne.server.getUrlNoParam
//airborne.server.setCookie(cname, cvalue, exhours)
// _server.get_root_domain();
airborne.server = {
	root_domain:null
	,get_root_domain:function(){
		return this.getRootDomain();
	}
	,getRootDomain:function(){

		if(airborne.validator.isValidStr(this.root_domain)){
			return this.root_domain;
		}
		// TODO IE not support 'origin'
		var port = window.location.port;
		this.root_domain = window.location.origin;
		if(!isNaN(parseInt(port))){
			this.root_domain += ":" + port;
		}

		return this.root_domain;
	}
	,get_parameterByName:function(name) {
	    name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
	    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
	        results = regex.exec(location.search);
	    return results == null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
	}
	,get_url_no_param:function(){
		return this.getUrlNoParam();
	}
	,is_valid_param_type:function(param_value) {
		// 파라미터로 넘길수 있는 것은 오직 3가지 타입만 가능합니다.
		if(	typeof param_value !== "number" && 
			typeof param_value !== "string" &&
			typeof param_value !== "boolean" ) {

			return false;
		}

		return true;
	}
	,get_custom_url_n_custom_params:function(request_url, request_param_obj){
		
		if(request_param_obj != null){
			var url_str = request_url + "?";	
			for (var key in request_param_obj) {
				var value = request_param_obj[key];
				if(!this.is_valid_param_type(value)) {
					continue;
				}
				
				url_str += key + "=" + value + "&";
			}
			return url_str;
		}

		return request_url;
	}
	,get_url_custom_params:function(request_param_obj){
		
		if(request_param_obj != null){
			var url_str = this.get_url_no_param() + "?";	
			for (var key in request_param_obj) {
				var value = request_param_obj[key];
				if(!this.is_valid_param_type(value)) {
					continue;
				}

				url_str += key + "=" + value + "&";
			}
			return url_str;
		}

		return _server.get_url_no_param();

	}
	,get_url_with_params:function(request_url, request_param_obj){
		
		if(request_param_obj != null){
			var url_str = request_url + "?";	
			for (var key in request_param_obj) {
				var value = request_param_obj[key];
				if(!this.is_valid_param_type(value)) {
					continue;
				}

				url_str += key + "=" + value + "&";
			}
			return url_str;
		}

		return request_url;
	}
	,call_url_with_custom_params:function(request_param_obj){
		var cur_url_str = this.get_url_custom_params(request_param_obj);
		var _v = airborne.validator;
		if(_v.isNotValidStr(cur_url_str)){
			console.log("!Error! / airborne.server / call_url_with_custom_params / _v.isNotValidStr(cur_url_str)");
			return;
		}
		window.location.href = cur_url_str;
	}
	,call_url_get:function(request_url, request_param_obj){
		var cur_url_str = this.get_custom_url_n_custom_params(request_url, request_param_obj);
		var _v = airborne.validator;
		if(_v.isNotValidStr(cur_url_str)){
			console.log("!Error! / airborne.server / call_url_with_custom_params / _v.isNotValidStr(cur_url_str)");
			return;
		}
		window.location.href = cur_url_str;
	}
	,call_url_post:function(form_obj_parent_jq, request_param_obj){

		var _html = airborne.html;
		var _server = airborne.server;

		var request_url = _server.get_url_no_param();
		var is_post = true;
		var form_obj = _html.get_form_obj(form_obj_parent_jq, request_url, is_post);

		// param_map
		if(request_param_obj != undefined) {
			for (var key in request_param_obj) {
			    var value = request_param_obj[key];
				if(!this.is_valid_param_type(value)) {
					continue;
				}

			    form_obj.add_param(key, value);
			}
		}

		form_obj.submit();
	}
	,post_url_with_params:function(request_url, request_param_obj){
		
		var _html = airborne.html;
		var _server = airborne.server;

		var is_post = true;
		var form_obj_parent_jq = $("body");
		var form_obj = _html.get_form_obj(form_obj_parent_jq, request_url, is_post);

		// param_map
		if(request_param_obj != undefined) {
			for (var key in request_param_obj) {
			    var value = request_param_obj[key];
				if(!this.is_valid_param_type(value)) {
					continue;
				}

			    form_obj.add_param(key, value);
			}
		}

		// console.log("post_url_with_params / form_obj ::: ",form_obj);

		form_obj.submit();

	}	
	,getUrlNoParam:function(){
		var self_location = location.href;
		var indexOfQuestionMark = self_location.indexOf("?");
		var next_location = self_location;
		if(indexOfQuestionMark > 0){
			next_location = self_location.slice(0,indexOfQuestionMark);
		}
		return next_location;
	}
	,an_hour_in_millisec:3600000// 60*60*1000
	,a_week_in_hours:168 //24*7
	,setCookie:function(cname, cvalue, exhours) {

		if(exhours == null || !(exhours > 0)) {
			exhours = 1;
		}

	    var d = new Date();
	    d.setTime(d.getTime() + (exhours*this.an_hour_in_millisec));
	    var expires = "expires="+d.toGMTString();
	    document.cookie = cname + "=" + cvalue + "; " + expires;
	}
	,delCookie:function(cname) {

		this.setCookie(
			// cookie name
			cname
			// cookie value
			, 0
			// expire hours
			, -1
		);

	    // document.cookie = cname + '=; expires=Thu, 01 Jan 1970 00:00:01 GMT;';
	}
	,getCookie:function(cname) {
	    var name = cname + "=";
	    var ca = document.cookie.split(';');
	    for(var i=0; i<ca.length; i++) {
	        var c = ca[i].trim();
	        if (c.indexOf(name) == 0) return c.substring(name.length,c.length);
	    }
	    return "";
	}
	,getCookieNumber:function(cname) {
		var cur_value = this.getCookie(cname);
		if(cur_value == undefined || isNaN(cur_value)) {
			return -1;
		}
		return parseInt(cur_value);
	}

}


airborne.phone = {
	MOBILE_PHONE_KR:1 	// airborne.phone.MOBILE_PHONE_KR
	,isValidMobilePhoneNumber:function(src_mobile_number, mobile_type){ // airborne.phone.isValidMobilePhoneNumber
		var _v = airborne.validator;
		if(mobile_type==null){
			console.log("!Error! / airborne.phone.isValidMobilePhoneNumber / mobile_type==null");
			return false;
		}

		if(this.MOBILE_PHONE_KR==mobile_type){
			var safe_phone_number_value = src_mobile_number.replace(/-/gi, "");

			if(_v.isNotNumberStr(safe_phone_number_value)){
				console.log("!Error! / airborne.phone.isValidMobilePhoneNumber / _v.isNotNumberStr(safe_phone_number_value) / ",safe_phone_number_value);
				return false;
			}else if(safe_phone_number_value.length!=11){
				console.log("!Error! / airborne.phone.isValidMobilePhoneNumber / safe_phone_number_value.length!=11");
				return false;
			}

			return true;
		}

		return false;
	}
	,getFormattedMobilePhoneNumber:function(src_mobile_number, mobile_type){ // airborne.phone.getFormattedMobilePhoneNumber
		var _v = airborne.validator;
		if(mobile_type==null){
			console.log("!Error! / airborne.phone.getFormattedMobilePhoneNumber / mobile_type==null");
			return "";
		}

		if(this.MOBILE_PHONE_KR==mobile_type){
			var formattedMobilePhoneNumber = src_mobile_number;
			// 01046135949 --> 010-4613-5949
			if(src_mobile_number.length==11){
				var head_number = src_mobile_number.slice(0,3);
				var body_number = src_mobile_number.slice(3,7);
				var tail_number = src_mobile_number.slice(7,11);
				formattedMobilePhoneNumber = head_number + "-" + body_number + "-" + tail_number;
			}
			
			return formattedMobilePhoneNumber;
		}

		return "";
	}
}

//airborne.html.getSafeText
//airborne.html.getSQLSafeText
//airborne.html.getUnSQLText
//airborne.html.getId
//airborne.html.getIdRandomTail
//airborne.html.get_id_auto_increase
//airborne.html.getTextHead
//airborne.html.restoreText
//airborne.html.getQueryStringSafeText
//airborne.html.getTapStr()
airborne.html = {
	getTapStr:function(){
		return "&nbsp;&nbsp;&nbsp;";	
	}
	,getSafeText:function(unsafe_text){
		if(airborne.validator.isNotValidStr(unsafe_text)){
			return "";
		}

		unsafe_text = this.getUnSQLText(unsafe_text);

		if(unsafe_text.indexOf("&") > -1){
			unsafe_text = unsafe_text.replace(/&/gi, "&amp;");
		}

		if(unsafe_text.indexOf(">") > -1){
			unsafe_text = unsafe_text.replace(/>/gi, "&gt;");
		}

		if(unsafe_text.indexOf("<") > -1){
			unsafe_text = unsafe_text.replace(/</gi, "&lt;");
		}

		return unsafe_text;
	}
	,get_decode_text:function(encoded_text){	// TODO wdjung

		if(airborne.validator.isNotValidStr(encoded_text)){
			return "";
		}

		var decoded_text = encoded_text;

		if(encoded_text.indexOf("&amp;") > -1){
			decoded_text = decoded_text.replace(/&amp;/gi, "&");
		}

		if(encoded_text.indexOf("&gt;") > -1){
			decoded_text = decoded_text.replace(/&gt;/gi, ">");
		}

		if(encoded_text.indexOf("&lt;") > -1){
			decoded_text = decoded_text.replace(/&lt;/gi, "<");
		}

		return decoded_text;

	}
	// @ Desc : GET 전송시 파라미터로 전달하기 위한 문자열로 변경해 줍니다.
	,getQueryStringSafeText:function(unsafe_text){
		// & --> %26
		if(airborne.validator.isNotValidStr(unsafe_text)){
			return "";
		}

		// escape "
		// escape '
		unsafe_text = this.getSQLSafeText(unsafe_text);

		// &amp; --> &
		// &gt; --> >
		// &lt; --> <
		unsafe_text = this.get_decode_text(unsafe_text);

		// escape '&'
		var safe_text = "";
		if(unsafe_text.indexOf("&") > -1){
			safe_text = unsafe_text.replace(/\&/gi, "%26");
		} else {
			safe_text = unsafe_text;
		}

		return safe_text;
	}
	,getJSONStrSafeText:function(unsafe_text){

		if(unsafe_text.indexOf("'") > -1){
			unsafe_text = unsafe_text.replace(/\'/gi, "\\\'");
		}

		if(unsafe_text.indexOf("\"") > -1){
			unsafe_text = unsafe_text.replace(/\"/gi, "\\\"");
		}

		return unsafe_text;

	}
	/*
		@ Scope : Public
		@ Desc 	: get safe text value for <span>${VALUE}</span>
	*/
	,getSafeHTMLInline:function(unsafe_text) {

		// \' --> '
		if(unsafe_text.indexOf("\\\'") > -1){
			unsafe_text = unsafe_text.replace(/\\\'/gi, "'");
		}

		// \" --> "
		if(unsafe_text.indexOf("\\\"") > -1){
			unsafe_text = unsafe_text.replace(/\\\'/gi, "'");
		}

		return unsafe_text;
	}
	,getUnSQLText:function(sql_safe_text){ // airborne.html.getUnSQLText
		if(sql_safe_text.indexOf("&lsquo;") > -1){
			sql_safe_text = sql_safe_text.replace(/\&lsquo\;/gi, "\'");
		}

		if(sql_safe_text.indexOf("&quot;") > -1){
			sql_safe_text = sql_safe_text.replace(/\&quot\;/gi, "\"");
		}

		return sql_safe_text;
	}
	,getSQLSafeText:function(unsafe_text){ // TODO wdjung 특수 문자 입력에 대한 제한을 두어야 함.
		if(airborne.validator.isNotValidStr(unsafe_text)){
			return "";
		}

		if(unsafe_text.indexOf("'") > -1){
			unsafe_text = unsafe_text.replace(/\'/gi, "&lsquo;");
		}

		if(unsafe_text.indexOf("\"") > -1){
			unsafe_text = unsafe_text.replace(/\"/gi, "&quot;");
		}

		return unsafe_text;
	}
	,replaceEmptySpace:function(target_text){
		if(target_text == null) return;

		if(!isNaN(target_text)) return;

		if(target_text.indexOf(" ") > -1){
			return target_text.replace(/ /gi, "_");
		}

		return target_text;
	}
	,replaceEmbrace:function(target_text){

		if(target_text == null) return;

		if(!isNaN(target_text)) return;

		var left_embrace_idx = target_text.indexOf("(");
		if(left_embrace_idx > -1){
			target_text = target_text.replace(/\(/gi, "_");
		}
		var right_embrace_idx = target_text.indexOf(")");
		if(right_embrace_idx > -1){
			if(right_embrace_idx==(target_text.length-1)){
				// 오른쪽 괄호가 문장 맨 마지막인 경우.
				target_text = target_text.replace(/\)/gi, "");
			} else {
				// 오른쪽 괄호가 문장 끝나기 전에 있는 경우.
				target_text = target_text.replace(/\)/gi, "_");	
			}
		}

		return target_text;

	}
	,getId:function(raw_id){ 
		// return only alphabet.
		if(!raw_id) return undefined;

		if(!isNaN(raw_id)) return raw_id;

		raw_id = raw_id.replace(/ /gi, "_");

	    var patt1 = /[a-z_0-9\-]/gi; 
	    var result = raw_id.toLowerCase().match(patt1);

	    if(result == null || result.length == 0) {
	    	return raw_id;
	    }
	    
	    return result.join("");
	}
	,cur_idx_ASC:0
	,getIdASCTail:function(raw_id){
		// 1씩 증가하는 아이디를 사용합니다.
		return this.getId(raw_id) + "_" + (this.cur_idx_ASC++);
	}
	,getIdRandomTail:function(raw_id){

		var d = new Date();
		var n = d.getTime();

		return this.getId(raw_id) + "_" + n + "_" + Math.floor((Math.random() * 1000000) + 1);
	}
	,auto_increase_num:0
	,get_id_auto_increase:function(raw_id){
		this.auto_increase_num += 1;
		return this.getId(raw_id) + "_" + this.auto_increase_num;
	}
	,get_num_id_auto_increase:function(raw_id){
		this.auto_increase_num += 1;
		return this.auto_increase_num + "_" + this.getId(raw_id);
	}
	,restoreText:function(safe_text){ // TODO wdjung

		if(airborne.validator.isNotValidStr(safe_text)){
			return "";
		}

		if(safe_text.indexOf("&amp;") > -1){
			safe_text = safe_text.replace(/&amp;/gi, "&");
		}

		return safe_text;
	}
	,getTextHead:function(src_text, limit){
		var text_head = "";
		if(airborne.validator.isNotValidStr(src_text)){
			return text_head;
		}
		if(airborne.validator.isNotNumber(limit)){
			return text_head;
		}

		if(src_text.length > limit){
			text_head = src_text.slice(0,limit) + "...";	
		} else {
			text_head = src_text;
		}
		return text_head;
	}
	,getPDFSafeTagText:function(raw_tag_text){ // airborne.html.getPDFSafeTagText
		var safe_tag_text = raw_tag_text;
		if(raw_tag_text.indexOf("'") > -1){
			safe_tag_text = raw_tag_text.replace(/'/gi, "\\'");
		}

		return safe_tag_text;
	}
	,get_form_obj:function(parent_jq, request_url, is_post){
		var _v = airborne.validator;
		if(parent_jq == null){
			console.log("!Error! / airborne.html.drawForm / parent_jq == null");
		}
		if(_v.isNotValidStr(request_url)){
			console.log("!Error! / airborne.html.drawForm / _v.isNotValidStr(request_url)");
		}
		var method = (is_post == null || is_post == false)?"GET":"POST";
		var form_tag = "";
		var form_tag_id = this.getIdRandomTail("form");

		form_tag += ""
		+ "<form role=\"form\" id=\"<form_tag_id>\" action=\"<request_url>\" method=\"<method>\">"
		.replace(/\<form_tag_id\>/gi, form_tag_id)
		.replace(/\<request_url\>/gi, request_url)
		.replace(/\<method\>/gi, method)
		+ "</form>";

		parent_jq.append(form_tag);
		var form_jq = parent_jq.find("form#" + form_tag_id);
		var _self = this;

		var form_obj = {
			parent_jq:parent_jq
			,request_url:request_url
			,is_post:is_post
			,form_tag_id:form_tag_id
			,form_jq:form_jq
			,add_param:function(key, value){
				var form_param_tag = "";

				// form POST 전송시, php에서 인자로 받는 경우 쌍따옴표를 json str으로 파싱하는 경우의 문제가 있습니다.
				// 이를 위해서 일반적인 &quot;을 사용하지 않고
				// 이 프로젝트에서만 사용하는 문자열로 치환합니다.
				if(typeof value === 'string') {
					var tm_quote = "TTTM_QUOTEEE";
					var tm_single_quote = "TTTM_SINGLE_QUOTEEE";
					value = value.replace(/\&quot\;/gi, tm_quote);
					value = value.replace(/\&lsquo\;/gi, tm_single_quote);
				}

				var safe_value_encoded = encodeURI(value);

				form_param_tag += 
				"<input type=\"hidden\" id=\"<id>\" name=\"<name>\" value=\"<value>\">"
				.replace(/\<id\>/gi, key)
				.replace(/\<name\>/gi, key)
				.replace(/\<value\>/gi, safe_value_encoded)
				;

				this.form_jq.append(form_param_tag);
			}
			,submit:function(){
				this.form_jq.submit();
			}
		}

		return form_obj;
	}
	/*
		@ Public
		@ Desc : 현재 문서의 스크롤 y 위치를 알려줍니다.
		@ Refer : http://stackoverflow.com/questions/3464876/javascript-get-window-x-y-position-for-scroll
	*/
	, get_top:function() {
		var doc = document.documentElement;
		var top = (window.pageYOffset || doc.scrollTop)  - (doc.clientTop || 0);

		return top;
	}
	/*
		@ Public
		@ Desc : 현재 문서의 스크롤 y 위치를 알려줍니다.
		@ Refer : http://stackoverflow.com/questions/3464876/javascript-get-window-x-y-position-for-scroll
	*/
	, get_left:function() {
		var doc = document.documentElement;
		var left = (window.pageXOffset || doc.scrollLeft) - (doc.clientLeft || 0);

		return left;
	}
	// @ Desc : "Chris&apos; corner" --> "Chris' corner"
	, get_html_entity_decode_safe:function(target_str) {

		var id = "html_entity_decode_safe";	
		var target_jq = $("textarea#" + id);
		if(target_jq.length < 1) {
			$("body").append(
				"<textarea id=\"${id}\" style=\"display:hidden;\"/>".replace(/\$\{id\}/gi, id)
			);
			target_jq = $("textarea#" + id);
		}
		if(target_jq.length < 1) {
			console.log("!Error! / get_html_entity_decode_safe / cannot find target jq!");
			return target_str;
		}

		var safe_text = target_jq.html(target_str).text();

		// 강제적으로 화면에 노출되는 경우는 화면에서 삭제.
		if(target_jq.is(":visible")) {
			target_jq.remove();
		}

		return safe_text;
	}	


}

// airborne.image.get_cropper(ele_canvas, crop_size);
airborne.image = {
	get_cropper:function(ele_canvas, crop_size, callback_on_load, dest_width, dest_height) {

		var cropper = {
			ele_canvas:ele_canvas
			, crop_size:crop_size
			, context:null
			, image_obj:null
			, callback_on_load:callback_on_load
			, dest_width:dest_width
			, dest_height:dest_height
			, img_url:null
			, init:function() {

				var context = this.context = this.ele_canvas.getContext('2d');
				var image_obj = this.image_obj = new Image();
				var crop_size = this.crop_size;
				var callback_on_load = this.callback_on_load;

				this.image_obj.onload = function() {

					// wonder.jung - 로딩 실패한 경우의 처리는 어떻게?

					// 원본의 이미지 크기를 알아야 한다.
					var src_width = parseInt(image_obj.width);
					var src_height = parseInt(image_obj.height);
					var src_x_pos = 0;
					var src_y_pos = 0;

					var dest_x_pos = 0;
					var dest_y_pos = 0;

					var dest_width = crop_size;
					if(this.dest_width != null && 0 < this.dest_width) {
						dest_width = this.dest_width;
					}
					var dest_height = crop_size;
					if(this.dest_height != null && 0 < this.dest_height) {
						dest_height = this.dest_height;
					}


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
					context.drawImage(image_obj, src_x_pos, src_y_pos, src_width, src_height, dest_x_pos, dest_y_pos, dest_width, dest_height);

					callback_on_load._apply([image_obj]);
				};
			}
			, resize:function() {

				// console.log("resize / this.img_url ::: ",this.img_url);

				// this.init();
				// this.load(this.img_url);

			}
			, load:function(img_url) {
				//imageObj.src = 'http://www.html5canvastutorials.com/demos/assets/darth-vader.jpg';
				this.img_url = img_url;
				this.image_obj.src = img_url;
			}
			, show:function() {
				$(ele_canvas).show();
			}
			, hide:function() {
				$(ele_canvas).hide();
			}
			, cropper_id:0
			, set_id:function(cropper_id) {
				this.cropper_id = cropper_id;
			}
			, get_id:function() {
				return this.cropper_id;
			}
		}
		cropper.init();

		return cropper;
	}

}

airborne.param = {
	get:function(param_name, param_value){
		var param_obj = {
			get:function(param_name, param_value) {
				this[param_name] = param_value;
				return this;
			}
		};

		return param_obj.get(param_name, param_value);
	}	
}


airborne.regex = {

	has:function(target_str, match_keyword) {

		if(_v.is_not_valid_str(target_str)) {
			console.log("airborne.regex.has / _v.is_not_valid_str(target_str)");
			return;
		}

		if(_v.is_not_valid_str(match_keyword)) {
			console.log("airborne.regex.has / _v.is_not_valid_str(match_keyword)");
			return;
		}

		var regex = new RegExp(match_keyword,"gi");
		var match_arr = target_str.match(regex);

		if(_v.is_valid_array(match_arr)) {
			return true;
		}
		return false;
	}
	, has_similar_name:function(target_str, match_keyword) {

		if(_v.is_not_valid_str(target_str)) {
			return false;
		}

		if(_v.is_not_valid_str(match_keyword)) {
			return false;
		}

		var regex = new RegExp(match_keyword,"gi");
		var match_arr = target_str.match(regex);

		if(_v.is_not_valid_array(match_arr)) {
			return false;
		}

		// 글자수가 키워드의 3배를 넘을수 없다.
		if((match_keyword.length * 3) < target_str.length) {
			// 문장에 포함된 이름. 
			return false;
		}

		return true;
		
	}
	, is_url:function(target_str) {

		if(_v.is_not_valid_str(target_str)) {
			return false;
		}

		var match_arr = target_str.match(/^(http|https).+/gi);

		if(_v.is_not_valid_array(match_arr)) {
			return false;
		}

		return true;
		
	}
	, is_image_url:function(target_str) {

		// wonder.jung / REFACTOR ME - javascript 내에서 이미지여부를 판별할 수 있을까? 서버의 도움을 받는 것은?
		// 확인하면서 이미지를 미리 다운로드 받는다면? 사용하지 않는다면 삭제.

		// 일반적인 이미지주소를 사용하지 않는 경우도 있음.
		// https://media.playstation.com/is/image/SCEA/overwatch-origins-edition-screen-02-ps4-us-03mar16?$MediaCarousel_Original$

		// service 내에서는 아래 이미지 타입만 사용하도록 강제함.

		if(_v.is_not_valid_str(target_str)) {
			return false;
		}

		var match_arr = target_str.match(/^(http|https).+\.(png|jpg|jpeg|gif)/gi);

		if(_v.is_not_valid_array(match_arr)) {
			return false;
		}

		return true;
		
	}
	, remove_empty_space:function(target_str) {

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}

		return target_str.replace(/\s/gi, "");

	}
	, remove_empty_nextline:function(target_str) {

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}

		// http://stackoverflow.com/questions/16369642/javascript-how-to-use-a-regular-expression-to-remove-blank-lines-from-a-string
		return target_str.replace(/^\s*[\r\n]/gm, "");
	}
	// # INPUT

	// "This is a Bengal cat.	Abyssinian cat.	X	https://en.wikipedia.org/wiki/Abyssinian_cat#/media/File:Gustav_chocolate.jpg"
	// ""
	// ""
	// ""

	// # OUTPUT

	// "This is a Bengal cat.	Abyssinian cat.	X	https://en.wikipedia.org/wiki/Abyssinian_cat#/media/File:Gustav_chocolate.jpg"
	, remove_begining_nextline:function(target_str) {

		// var test = _regex.remove_end_nextline("\nABC\nEFG\nHIJ\n\n\nLLL");
		// console.log("test ::: ",test);

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}

		// http://stackoverflow.com/questions/16369642/javascript-how-to-use-a-regular-expression-to-remove-blank-lines-from-a-string
		return target_str.replace(/^$[\r\n]/gm, "");
	}
	, remove_end_nextline:function(target_str) {

		// var test = _regex.remove_end_nextline("\nABC\nEFG\nHIJ\n\n\nLLL");
		// console.log("test ::: ",test);

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}

		// http://stackoverflow.com/questions/16369642/javascript-how-to-use-a-regular-expression-to-remove-blank-lines-from-a-string
		return target_str.replace(/$[\r\n]^$/gm, "");
	}
	, add_empty_space_before_quote_at_begining:function(target_str) {

		// Papa parser는 문장 첫부분의 Double Quote을 에러로 처리합니다.
		// 이를 우회하기 위해서 Double Quote에 공백을 입력, 문제가 없는 것로 인식하게 합니다.

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}

		// https://github.com/mholt/PapaParse/issues/226
		return target_str.replace(/^(["'])/gm, " $1");
	}
	, remove_empty_spaces_at_begining:function(target_str) {

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}
		return target_str.replace(/^\s+/gi, "");
	}
	, remove_empty_spaces_at_end:function(target_str) {

		if(_v.is_not_valid_str(target_str)) {
			return target_str;
		}
		return target_str.replace(/\s+$/gi, "");
	}



}

airborne.array = {
	remove_element:function(target_value, target_array) {

		if(target_value == null) {
			return;
		}

		if(_v.is_not_valid_array(target_array)) {
			return;
		}

		for(var idx=0; idx < target_array.length; idx++) {
			if(target_array[idx] !== target_value) {
				continue;
			}
			return target_array.splice(idx,1);
		}

		return target_array;
	}	
}



