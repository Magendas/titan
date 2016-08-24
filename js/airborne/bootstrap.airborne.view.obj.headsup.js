// clouser practice.
// Airbonre Collection View Input - refer to bootstrap.airborne.view.obj.input.js
var abc_headsup = (function() {

	// private scope
	// variables
	var headsup_jq;
	var title_jq;
	var msg_jq;

	// private scope
	// functions
	function set_headsup(target_jq) {
		headsup_jq = target_jq;
		title_jq = headsup_jq.find("span#title");
		msg_jq = headsup_jq.find("span#msg");
	}

	function set_title(new_title) {
		if(new_title != null && new_title != "") {
			title_jq.html(new_title);
		}
	}

	function set_msg(new_msg) {
		if(new_msg != null && new_msg != "") {
			msg_jq.html(new_msg);
		}
	}

	function change_warning() {
		headsup_jq.removeClass("btn-info");
		headsup_jq.removeClass("btn-success");
		headsup_jq.removeClass("btn-warning");
		headsup_jq.removeClass("btn-danger");

		headsup_jq.addClass("btn-warning");
	}

	function jump_z() {
		headsup_jq.css("z-index","5000");
	}

	function show_headsup() {
		headsup_jq.show();
		headsup_jq.click(function(e){
			e.preventDefault();
			e.stopPropagation();

			headsup_jq.hide();
		});
	}

	function hide_headsup() {
		headsup_jq.hide();	
	}
	function set_css(key, value) {
		headsup_jq.css(key, value);
	}
	function addClass_headsup(className) {
		headsup_jq.addClass(className);	
	}

	// public scope
	return {		
	    set: function(target_jq) {
	      set_headsup(target_jq);
	    }
	    , _show_warning:function(title, msg) {
	    	change_warning();
	    	this._show(title, msg);
	    }
	    , _show:function(title, msg, scope, callback_after_show) {
	    	show_headsup();
	    	set_title(title);
	    	set_msg(msg);

			if(scope != null && callback_after_show != null) {
				setTimeout(function(){
					console.log("3 초후에 이동합니다.");
					callback_after_show.apply(scope, []);	
				}, 3000);
			}

	    }
	    , _hide:function() {
	    	hide_headsup();
	    }
	    , _jump:function() {
	    	jump_z();
	    }
	    , _css:function(key, value) {
	    	set_css(key, value);
	    }
	    , _addClass:function(className) {
	    	addClass_headsup(className);
	    }
	}

})();
