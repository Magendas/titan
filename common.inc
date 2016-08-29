<?php

date_default_timezone_set("Asia/Seoul");

// MODEL - MYSQL
include_once(__DIR__."/inc/mysql/mysql.config.inc");
include_once(__DIR__."/inc/mysql/mysql.interface.inc");
include_once(__DIR__."/inc/mysql/mysql.manager.inc");
include_once(__DIR__."/inc/mysql/mysql.query.manager.inc");
include_once(__DIR__."/inc/mysql/mysql.feedback.inc");

// VIEW
include_once(__DIR__."/inc/view/button.manager.inc");
include_once(__DIR__."/inc/view/checkbox.manager.inc");
include_once(__DIR__."/inc/view/combobox.manager.inc");
include_once(__DIR__."/inc/view/input.manager.inc");
include_once(__DIR__."/inc/view/page.manager.inc");
include_once(__DIR__."/inc/view/headsup.manager.inc");
include_once(__DIR__."/inc/view/view.renderer.inc");

// VIEW - ACTION
include_once(__DIR__."/inc/action/action.manager.inc");
include_once(__DIR__."/inc/action/action.template.manager.inc");

// CONTROLLER - SERVER
include_once(__DIR__."/inc/server/api.postprocessor.inc");
include_once(__DIR__."/inc/server/cookie.manager.inc");

// CONTROLLER - COMMON
include_once(__DIR__."/inc/common/time.manager.inc");
include_once(__DIR__."/inc/common/checker.inc");
include_once(__DIR__.'/inc/common/array.manager.inc');

// CONTROLLER - SERVICE
include_once(__DIR__."/inc/service/link.manager.inc");
include_once(__DIR__."/inc/service/fetch.json.inc"); // json 파일을 읽어 stdClass 객체로 반환.
include_once(__DIR__."/inc/service/param.manager.inc");
include_once(__DIR__."/inc/service/thumbnail.manager.inc");
include_once(__DIR__."/inc/service/image.upload.manager.inc");

include_once(__DIR__."/inc/service/preprocessor.inc");
include_once(__DIR__."/inc/service/view.manager.inc");

// @ required
// 서비스에서 사용되는 고정 상수 값들
// constant parameters using in service.
$file_path_const = __DIR__."/js/service/const.json";
$const = FetchJSON::get($file_path_const);
if(is_null($const)) {
	echo "common.inc / is_null(\$const)<br/>";
	die();
}
$const = FetchJSON::get($file_path_const);
$file_path_const = null;

// 서비스에서 사용되는 고정 상수 값들 - Error
// constant parameters using in service.
$file_path_const_error = __DIR__."/js/service/const.error.json";
$const_error = FetchJSON::get($file_path_const_error);
if(is_null($const_error)) {
	echo "common.inc / is_null(\$const_error)<br/>";
	die();
}
$file_path_const_error = null;

// 서비스에서 사용되는 변수 명들
// variable names using in service.
$file_path_var_name = __DIR__."/js/service/var.name.json";
$var_name = FetchJSON::get($file_path_var_name);
if(is_null($var_name)) {
	echo "common.inc / is_null(\$var_name)<br/>";
	die();
}
$file_path_var_name = null;

// @ requried - titan param
$mysql_feedback = new MYSQLFeedback("common.inc");
if(is_null($mysql_feedback)) {
	echo "common.inc / is_null(\$mysql_feedback)<br/>";
	die();
}

$titan_param = ParamManager::get($mysql_feedback, $const, $var_name);
if(is_null($titan_param)) {
	echo "common.inc / is_null(\$titan_param)<br/>";
	die();
}

// @ requried - DB Setting
$mysql_config = MYSQLConfig::get_config_default();
$mysql_manager = new MYSQLManager($mysql_config);
$mysql_interface = new MYSQLInterface($mysql_manager, $titan_param, $mysql_feedback);


// @ required
// get service root path - V2
$path = substr( __FILE__, strlen( $_SERVER[ 'DOCUMENT_ROOT' ] ) );
$path_matches_group = array();
preg_match_all('/[\.a-zA-Z0-9_-]+/', $path, $path_matches_group);
$path_matches = $path_matches_group[0];

$service_root_path = "";
for($idx=0; $idx < (count($path_matches) - 1); $idx++) {
	// 마지막 인자는 자신의 파일이름이므로 제외합니다.
	$path_match = $path_matches[$idx];
	$service_root_path .= "/$path_match";
}
$server_root_path = 
(!empty($_SERVER['HTTPS']) ? 'https' : 'http') . '://' . $_SERVER['HTTP_HOST'] . $service_root_path;

// get full file path
$file_root_path = __DIR__;

// @ required - Titan Link Manager
TitanLinkManager::set_service_root_path($service_root_path);
TitanLinkManager::set_file_root_path($file_root_path);

// @ required - Titan Thumbnail Manager
TitanThumbnailManager::set_service_root_path($service_root_path);


// FETAL ERROR HANDLING - INIT
set_error_handler('TitanErrorHandler');
register_shutdown_function('fatalErrorShutdownHandler');

function TitanErrorHandler($code, $message, $file, $line) {

	// Consts, variables
	$file_path_const = __DIR__."/js/service/const.json";
	$const = FetchJSON::get($file_path_const);
	if(is_null($const)) {
		echo "TitanErrorHandler / is_null(\$const)<br/>";
		die();
	}

	$file_path_var_name = __DIR__."/js/service/var.name.json";
	$var_name = FetchJSON::get($file_path_var_name);
	if(is_null($var_name)) {
		echo "TitanErrorHandler / is_null(\$var_name)<br/>";
		die();
	}

	$mysql_feedback = new MYSQLFeedback(__FUNCTION__);
	if(is_null($mysql_feedback)) {
		echo "TitanErrorHandler / is_null(\$mysql_feedback)<br/>";
		die();
	}

	$titan_param = 
	ParamManager::get(
		// $feedback=null
		$mysql_feedback
		// $service_const_json=null
		, $const
		// $service_var_name_json=null
		, $var_name
	);
	if(is_null($titan_param)) {
		echo "TitanErrorHandler / is_null(\$titan_param)<br/>";
		die();
	}

	// DB Setup
	$mysql_config = MYSQLConfig::get_config_default();
	$mysql_manager = new MYSQLManager($mysql_config);
	$mysql_interface = 
	new MYSQLInterface(
		// $new_mysql_manager=null
		$mysql_manager
		// $param_manager=null
		, $titan_param
		// $feedback_manager=null
		, $mysql_feedback
	);
	if(is_null($titan_param)) {
		echo "TitanErrorHandler / is_null(\$mysql_interface)<br/>";
		die();
	}

	$CLIENT_IP = Checker::get_client_ip();
	$CLIENT_OS = Checker::get_client_os();;
	$CLIENT_BROWSER = Checker::get_client_browser();

	$query_param = new stdClass();
	$query_param->{$var_name->CLIENT_OS} = $CLIENT_OS;
	$query_param->{$var_name->CLIENT_IP} = $CLIENT_IP;
	$query_param->{$var_name->CLIENT_BROWSER} = $CLIENT_BROWSER;
	$query_param->{$var_name->ERROR_TYPE} = $const->ERROR_TYPE_FETAL_ERROR;

	$error_obj = new stdClass();
	$error_obj->code = $code;
	$error_obj->message = $message;
	$error_obj->file = $file;
	$error_obj->line = $line;
	$error_obj->time = TimeManager::get_now_micro();

	$query_param->{$var_name->ERROR_MSG} = json_encode($error_obj);

	$mysql_interface->insert_error_log($query_param);

	$mysql_interface->close();

	// TODO : email to webmaster.

};

function fatalErrorShutdownHandler(){
  $last_error = error_get_last();
  if ($last_error['type'] === E_ERROR) {
    // fatal error
    TitanErrorHandler(E_ERROR, $last_error['message'], $last_error['file'], $last_error['line']);
  }
}
// FETAL ERROR HANDLING - DONE

?>