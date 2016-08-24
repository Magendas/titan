<?php

class MYSQLConfig{

	// http://stackoverflow.com/questions/1676688/php-mysql-connection-not-working-2002-no-such-file-or-directory
	// Configuring PHP5 with MySQL support - /etc/php.ini
	// mysql.default_socket = /tmp/mysql.sock

	public static $MYSQL_CONFIG_DEV = "titan-dev";
	public static $MYSQL_CONFIG_DEV_HOST = "localhost";
	public static $MYSQL_CONFIG_DEV_USERNAME = "titan";
	public static $MYSQL_CONFIG_DEV_PASSWORD = "xkdlxks!1";
	public static $MYSQL_CONFIG_DEV_DATABASE = "titan";

	public static $MYSQL_CONFIG_STAGE = "titan-stage";
	public static $MYSQL_CONFIG_STAGE_HOST = "";
	public static $MYSQL_CONFIG_STAGE_USERNAME = "titan";
	public static $MYSQL_CONFIG_STAGE_PASSWORD = "xkdlxks!1";
	public static $MYSQL_CONFIG_STAGE_DATABASE = "titan";

	public static $MYSQL_CONFIG_PRODUCTION = "titan-production";
	public static $MYSQL_CONFIG_PRODUCTION_HOST = "";
	public static $MYSQL_CONFIG_PRODUCTION_USERNAME = "titan";
	public static $MYSQL_CONFIG_PRODUCTION_PASSWORD = "xkdlxks!1";
	public static $MYSQL_CONFIG_PRODUCTION_DATABASE = "titan";

	public static function getConfig($config_name){

		if($config_name == null || $config_name == "") return null;

		$config = null;
		if($config_name == self::$MYSQL_CONFIG_DEV){

			$config = new stdClass();
			$config->host = self::$MYSQL_CONFIG_DEV_HOST;
			$config->username = self::$MYSQL_CONFIG_DEV_USERNAME;
			$config->password = self::$MYSQL_CONFIG_DEV_PASSWORD;
			$config->database = self::$MYSQL_CONFIG_DEV_DATABASE;

		} else if($config_name == self::$MYSQL_CONFIG_STAGE){

			$config = new stdClass();
			$config->host = self::$MYSQL_CONFIG_STAGE_HOST;
			$config->username = self::$MYSQL_CONFIG_STAGE_USERNAME;
			$config->password = self::$MYSQL_CONFIG_STAGE_PASSWORD;
			$config->database = self::$MYSQL_CONFIG_STAGE_DATABASE;

		} else if($config_name == self::$MYSQL_CONFIG_PRODUCTION){

			$config = new stdClass();
			$config->host = self::$MYSQL_CONFIG_PRODUCTION_HOST;
			$config->username = self::$MYSQL_CONFIG_PRODUCTION_USERNAME;
			$config->password = self::$MYSQL_CONFIG_PRODUCTION_PASSWORD;
			$config->database = self::$MYSQL_CONFIG_PRODUCTION_DATABASE;

		}

		return $config;
	}

	public static function get_config_default() {
		return MYSQLConfig::getConfig(MYSQLConfig::$MYSQL_CONFIG_DEV);
	}

}

?>
