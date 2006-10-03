<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ezmlm.inc,v 1.32 2006-10-03 18:46:47 turbo Exp $
class ezmlm {
	// {{{ Private section
	var $_option					= array();
	var $_paths						= array();

	// {{{ _remote_list_setup(remote)
	function _remote_list_setup($remote) {
	  $cmd = (pql_get_define("PQL_CONF_EZMLM_REMOTE_RETREIVE_SCRIPT") ? pql_get_define("PQL_CONF_EZMLM_REMOTE_RETREIVE_SCRIPT") : '');
	  $usr = (pql_get_define("PQL_CONF_EZMLM_REMOTE_USER") ? pql_get_define("PQL_CONF_EZMLM_REMOTE_USER") : '');
	  $hst = (pql_get_define("PQL_CONF_EZMLM_REMOTE_HOST") ? pql_get_define("PQL_CONF_EZMLM_REMOTE_HOST") : '');
	  
	  if(!empty($cmd) and !empty($usr) and !empty($hst))
		return("env -i $cmd $hst $usr $remote");
	  else
		return("$remote");
	}
// }}}

	// {{{ _chmod(file, mode)
	function _chmod($file, $mode) {
	  if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		unset($out); unset($res);
		if($cmd = $this->_remote_list_setup("chmod $mode $file")) {
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			echo "CMD (_chmod): '$cmd'<br>";

		  $ret = @exec($cmd, $out, $res);
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			printr($out);

		  if(is_array($out) and !$res)
			$entries = $out;
		}
	  } else
		chmod($file, $mode);
	}
	// }}}

	// {{{ _chown(file, user)
	function _chown($file, $user) {
	  if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		unset($out); unset($res);
		if($cmd = $this->_remote_list_setup("chown $user $file")) {
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			echo "CMD (_chown): '$cmd'<br>";

		  $ret = @exec($cmd, $out, $res);
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			printr($out);

		  if(is_array($out) and !$res)
			$entries = $out;
		}
	  } else
		// TODO: We don't have access to do this!!!!
		//chown($file, $owner);
		return true;
	}
	// }}}

	// {{{ _getfsentries(dir)
	// Get filesystem entries (files or directories) recursively.
	function _getfsentries($dir) {
	  if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		unset($out); unset($res);
		if($cmd = $this->_remote_list_setup("find $dir -type d")) {
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			echo "CMD (_getfsentries): '$cmd'<br>";

		  $ret = @exec($cmd, $out, $res);
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			printr($out);

		  if(is_array($out) and !$res)
			$entries = $out;
		}
	  } else {
		if(!(($directory = @opendir($dir)) !=0)) {
			$this->error = "could not open the directory '$dir'";
			return false;
		}
		
		for(;GetType($entry = readdir($directory)) == "string";) {
			if($entry == "." || $entry == "..")
			  continue;
			
			if(@is_dir($dir."/".$entry)) {
				// directory - recursive invocation
				
				$e = $this->_getfsentries($dir . "/" . $entry);
				for($i=0; $i < count($e); $i++) {
					$entries[] = $e[$i];
				}
			}

			// First the file above, then the directory.
			// That way we get it sorted and done for
			// deletion!
			$entries[] = $dir . "/" . $entry;
		}

		closedir($directory);
	  }

	  return $entries;
	}
// }}}
	
	// {{{ _getdotlinks(dir, base)
	// Get DOTqmail links linked to specified directory
	function _getdotlinks($dir, $base) {
		// We're looking for some file that starts with this regexp:
		//		$dir.*/$base
		// And ends with any of these:
		$regexp[] = '$';
		$regexp[] = '-accept-default';
		$regexp[] = '-default';
		$regexp[] = '-digest-owner';
		$regexp[] = '-digest-return-default';
		$regexp[] = '-owner';
		$regexp[] = '-reject-default';
		$regexp[] = '-return-default';

		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup("find $dir -type l")) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (_getdotlinks): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
			
			if(is_array($out) and !$res) {
			  for($j=0; $j <= count($out); $j++) {
				for($i=0; $i < count($regexp); $i++) {
				  $lookfor = $dir.'.*/'.$base.$regexp[$i];
				  
				  if(ereg($lookfor, $out[$j]))
					$entries[] = $out[$j];
				}
			  }
			}
		  }
		} else {
		  if(!(($directory = @opendir($dir)) !=0)) {
			$this->error = "could not open the directory '$dir'";
			return false;
		  }
		
		  for(;GetType($entry = readdir($directory)) == "string";) {
			if($entry == "." || $entry == "..")
			  continue;

			$target = $dir . "/" . $entry;
			if(@is_link($target)) {
				// It's a link.

				for($i=0; $i < count($regexp); $i++) {
					$lookfor = $dir.'.*/'.$base.$regexp[$i];

					if(ereg($lookfor, $target)) {
						$entries[] = $target;
					}
				}
			}
		  }

		  closedir($directory);
		}

		return $entries;
	}
// }}}

	// {{{ _readlists(loadtexts)
	// load list of mailinglists in directory
	function _readlists($loadtexts = 0) {
	  if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		if(! ereg("/$", $user_home))
		  $list_path = ($this->user_home.'\\*');
		else
		  $list_path = ($this->user_home.'/\\*');

		unset($out); unset($res);
		if($cmd = $this->_remote_list_setup("find $list_path -type d -maxdepth 0")) {
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			echo "CMD (_readlists): '$cmd'<br>";

		  $ret = @exec($cmd, $out, $res);
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			printr($out);

		  if(is_array($out) and !$res) {
			$list_index = count($out);

			for($i=0; $i < count($out); $i++) {
			  $entry_name = basename($out[$i]);
			  if(!$this->_readlistconfiguration($entry_name,
												$this->mailing_lists[$i],
												$loadtexts))
				// Can't read list configuration
				return false;
			  
			  $this->mailing_lists_index[$entry_name] = $i;
			}
		  }
		} else {
		  $this->error="could not open the ezmlm user home directory";
		  return false;
		}
	  } else {
		if(!(($directory = @opendir($this->user_home)) !=0)) {
			$this->error="could not open the ezmlm user home directory";
			return false;
		}

		for(;GetType($entry_name = readdir($directory)) == "string";) {
			if((strcmp(substr($entry_name, 0, 1),".") &&
				@is_dir($this->user_home."/".$entry_name) &&
				file_exists($this->user_home."/".$entry_name."/config")))
			  {
				  $list_index = count($this->mailing_lists);
				  if(!$this->_readlistconfiguration($entry_name,
													$this->mailing_lists[$list_index],
													$loadtexts))
					{
						closedir($directory);
						return false;
					}

				  $this->mailing_lists_index[$entry_name] = $list_index;
			  }
		}

		closedir($directory);
	  }

	  if($this->mailing_lists_index) {
		ksort($this->mailing_lists_index);
		foreach($this->mailing_lists_index as $name => $no) {
		  $host = $this->mailing_lists[$no]["host"];
		  $this->mailing_lists_hostsindex[$host][$name] = $no;
		}
		
		$this->mailing_lists_hostsindex["COUNT"] = count($this->mailing_lists_hostsindex[$host]);
		return true;
	  } else
		return false;
	}
// }}}

	// {{{ _readlistconfiguration(list, configuration, loadtexts)
	// Read list configuration
	function _readlistconfiguration($list, &$configuration, $loadtexts) {
		$configuration["name"] = $list;

		if(!ereg("/$", $this->user_home))
		  $list_path = ($this->user_home.$list);
		else
		  $list_path = ($this->user_home."/$list");

		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup("cat ".$list_path."/config")) {
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			echo "CMD (_readlistconfiguration/1): '$cmd'<br>";

			$ret = @exec($cmd, $out, $res);

			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
			
			if(is_array($out) and !$res) {
			  $CONFIG = $out;
			}
		  }
		} else {
		  if(!( ($file = @fopen($list_path."/config", "r")) !=0)) {
			$this->error="could not open the mailing list configuration file";
			return false;
		  }

		  for(;!feof($file);) {
			if(!(GetType($line = fgets($file, 256)) == "string")) {
				if(feof($file)) {
					break;
				}

				fclose($file);
				$this->error="could not read the mailing list configuration file";
				return false;
			}

			if( (!GetType($colon = strpos($line, ":")) == "integer" || $colon == 0) ) {
			  fclose($file);
			  $this->error = "invalid mailing list configuration file";
			  return false;
			}

			$CONFIG[] = $line;
		  }

		  fclose($file);
		}

		for($i=0; $i < count($CONFIG); $i++) {
			$line = $CONFIG[$i];

			if( (GetType($endofline = strpos($line, "\n")) == "integer") )
			  $line = substr($line, 0, $endofline);
			
			if( (GetType($endofline = strpos($line, "\r")) == "integer") )
			  $line = substr($line, 0, $endofline);

			if(pql_get_define("PQL_CONF_EZMLM_REMOTE"))
			  $colon = strpos($line, ":");
			
			$value = substr($line, ($colon+1));
			switch(substr($line, 0, $colon)) {
			  case "0":
				$configuration["listparent"]	= $value;
				break;

			  case "3":
				$configuration["fromaddress"]	= $value;
				break;

			  case "5":
				$configuration["owner"]			= $value;
				break;

			  case "D":
				$configuration["directory"]		= $value;
				break;

			  case "F":
				foreach($this->_option as $attrib => $x) {
					$configuration[$attrib] = GetType(strpos($value, $this->_option[$attrib]["on"])) == "integer";
				}
				break;

			  case "H":
				$configuration["host"]			= $value;
				break;

			  case "L":
				$configuration["local"]			= $value;
				break;

			  case "T":
				$configuration["dotpath"]		= $value;
				break;
			}
		}

		// ------------------------
		// Get subscriber addresses
		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  $command = $this->_paths["list"] . " $list_path";

		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup($command)) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (_readlistconfiguration/2): '$cmd'<br>";
			$ret = @exec($cmd, $out, $res);

			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
		  }
		} else {
		  $command = $this->_paths["list"] . " 2>&1 " . $list_path;
		  $output = @exec($command, $out, $res);
		  if($res != 0) {
			$this->error = "could not execute system command: '" . $command . "'" .
			  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
			return false;
		  }
		}

		$configuration["subscribers"] = count($out);
		for($i=0; $i < count($out); $i++)
		  $configuration["subscriber"][] = $out[$i];

		// ------------------------
		// Get deny addresses
		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup("file $list_path/deny/lock")) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (_readlistconfiguration/3a): '$cmd'<br>";

			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);

			if(eregi('No such file or directory', $out[0])) {
			  unset($out); unset($res);
			} else {
			  unset($out); unset($res);
			  if($cmd = $this->_remote_list_setup($this->_paths["list"]." $list_path/deny")) {
				if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
				  echo "CMD (_readlistconfiguration/3b): '$cmd'<br>";

				$ret = @exec($cmd, $out, $res);
				if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
				  printr($out);
			  }
			}
		  }
		} else {
		  if(file_exists($list_path . "/deny/lock")) {
			unset($out); unset($res);

			$command = $this->_paths["list"] . " 2>&1 " . $list_path . "/deny";
			$output = @exec($command, $out, $res);
			if($res != 0) {
				$this->error = "could not execute system command: '" . $command . "'" .
				  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
				return false;
			}
		  }
		}

		$configuration["killlisted"] = count($out);
		for($i=0; $i < count($out); $i++)
		  $configuration["killlist"][] = $out[$i];

		if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
		  echo "LIST '$list':"; printr($configuration);
		  echo "-----------<p>";
		}

		// ------------------------
		// TODO: How do we 'remotify' this!?!?
		if($loadtexts) {
			$configuration["texts"] = array("top"				=> HtmlEntities("Administration-message-header"),
											"bottom"			=> HtmlEntities("Administration-message-footer"),
											"sub-confirm"		=> HtmlEntities("Subscription-confirmation-request"),
											"sub-ok"			=> HtmlEntities("Subscription-approved"),
											"sub-nop"			=> HtmlEntities("Already-subscribed-notice"),
											"sub-bad"			=> HtmlEntities("Invalid-subscription-confirmation"),
											"unsub-confirm"		=> HtmlEntities("Unsubscription-confirmation-request"),
											"unsub-ok"			=> HtmlEntities("Unsubscription-approved"),
											"unsub-nop"			=> HtmlEntities("Already-unsubscribed-notice"),
											"unsub-bad"			=> HtmlEntities("Invalid-unsubscription-confirmation"),
											"trailer"			=> HtmlEntities("Message-trailer"),
											"mod-sub-confirm"	=> HtmlEntities("Confirm-moderated-subscription"),
											"mod-unsub-confirm"	=> HtmlEntities("Confirm-moderated-unsubscription"));
		}

		return true;
	}
// }}}

	// {{{ _verifysubscriber(list, subscriber)
	// Verify subscriber (is the address subscribed?)
	function _verifysubscriber($list, $subscriber) {
		$list_path = $this->user_home."/".$list;

		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup($this->_paths["list"]." $list_path")) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (_verifysubscriber): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
		  }
		} else {
		  $command   = $this->_paths["list"] . " 2>&1 ".$list_path;
		  $output    = @exec($command, $out, $res);
		  if($res != 0) {
			$this->error = "Could not execute system command: ".$command
			  . ((!strcmp($output,"")) ? "" : (" (".$output.")"));
			return false;
		  }
		}

		$subscriber_address = strtolower($subscriber);
		for($address=0; $address < count($out); $address++) {
		  if(! strcmp($subscriber_address, strtolower($out[$address]))) 
			return true;
		}

		return false;
	}
// }}}

	// {{{ _chmodown(directory, username, dirmode, filemode)
	// Chown and Chmod directory recursive
	function _chmodown($directory, $username, $dirmode = '0770', $filemode = '0660') {
		// Find user id for the username
		$uidnr = @posix_getpwnam($username);
		$uidnr = $uidnr["uid"];

		umask(0011);

		_chmod($directory, octdec($dirmode));

		$files = $this->_getfsentries($directory);
		for($i=0; $i < count($files); $i++) {
			if(is_dir($files[$i]))
			  _chmod($files[$i], octdec($dirmode));
			else
			  _chmod($files[$i], octdec($filemode));

			_chown($files[$i], $uidnr);
		}

//		putenv("PQL_USERGROUP=$uidnr.".posix_getuid());
//		putenv("PQL_DIRECTORY=".$directory);
//		pql_execute('./sub_ezmlm_create.pl', 1);
	}
// }}}
// }}}

	// {{{ Public section
	var $user_name					= "";
	var $user_home					= "";

	var $error_message				= "";
	var $error						= "";
	var $debug						= 0;

	var $mailing_lists				= array();
	var $mailing_lists_index		= array();
	var $mailing_lists_hostindex	= array();

	// {{{ ezmlm(user_name, user_home, paths)
	// Constructor. Intialize the variable array of
	// attribute and what command line option to ezmlm-make
	// this corresponds to when on or off.
	// Couldn't do this 'globaly', at variable declaration
	// above, so I had to do it in the costructor. I don't
	// have a better use for it anyway :)
	function ezmlm($user_name = 'newsletters', $user_home = '/home/newsletters', $cmd_paths = 0) {
		$this->_option["archived"]["on"]		= "a";	$this->_option["archived"]["off"]		= "A";
		$this->_option["remotecfg"]["on"]		= "c";	$this->_option["remotecfg"]["off"]		= "C";
		$this->_option["digest"]["on"]			= "d";	$this->_option["digest"]["off"]			= "D";
		$this->_option["prefix"]["on"]			= "f";	$this->_option["prefix"]["off"]			= "F";
		$this->_option["guardedarchive"]["on"]	= "g";	$this->_option["guardedarchive"]["off"]	= "G";
		$this->_option["subhelp"]["on"]			= "h";	$this->_option["subhelp"]["off"]		= "H";
		$this->_option["indexed"]["on"]			= "i";	$this->_option["indexed"]["off"]		= "I";
		$this->_option["subjump"]["on"]			= "j";	$this->_option["subjump"]["off"]		= "J";
		$this->_option["kill"]["on"]			= "k";	$this->_option["kill"]["off"]			= "";
		$this->_option["sublistable"]["on"]		= "l";	$this->_option["sublistable"]["off"]	= "L";
		$this->_option["moderated"]["on"]		= "m";	$this->_option["moderated"]["off"]		= "M";
		$this->_option["modonly"]["on"]			= "o";	$this->_option["modonly"]["off"]		= "O";
		$this->_option["public"]["on"]			= "p";	$this->_option["public"]["off"]			= "";
		$this->_option["private"]["on"]			= "P";	$this->_option["private"]["off"]		= "";
		$this->_option["reqaddress"]["on"]		= "q";	$this->_option["reqaddress"]["off"]		= "Q";
		$this->_option["remoteadm"]["on"]		= "rn";	$this->_option["remoteadm"]["off"]		= "RN";
		$this->_option["submoderated"]["on"]	= "s";	$this->_option["submoderated"]["off"]	= "S";
		$this->_option["trailers"]["on"]		= "t";	$this->_option["trailers"]["off"]		= "T";
		$this->_option["subonly"]["on"]			= "u";	$this->_option["subonly"]["off"]		= "U";
		$this->_option["extras"]["on"]			= "x";	$this->_option["extras"]["off"]			= "X";

		// This SHOULD allow us to be able to 'replace' the commands with something
		// that will do remote configuration - scripts that uses rsh/ssh perhaps?
		if(! $cmd_paths) {
			$this->_paths["make"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-make";
			$this->_paths["list"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-list";
			$this->_paths["sub"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-sub";
			$this->_paths["unsub"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-unsub";
		} else {
			// Assign EXISTING values
			foreach($cmd_paths as $key => $val)
			  $this->_paths[$key] = $cmd_paths[$key];

			// Assign EXISTING values with defaults
			$keys = array('make', 'list', 'sub', 'unsub');
			foreach($keys as $key) {
				if(! $this->_paths[$key]) 
				  $this->_paths[$key] = pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-" . $key;
			}
		}
		
		$this->user_name = $user_name;

		if(! ereg("/$", $user_home))
		  $this->user_home = $user_home."/mailinglists/";
		else
		  $this->user_home = $user_home."mailinglists/";

		// Load list of mailinglists
		$this->_readlists();
	}
// }}}

	// {{{ updatelistentry(add, listname, domain, options, curropts)
	// Create/Modify mailinglist
	function updatelistentry($add, $listname, $domain, $options, $curropts=0) {
		// * If we should create a list, set $add to 1/TRUE
		//   and $options should contain the options to enable.
		//
		// * To modify (ie, toggle) a value on an existing list,
		//   set $add to 0/FALSE, $options to the attribute to
		//   change and $curropts to the current list options

		// {{{ Setup options to ezmlm-make
		if($add) {
			// Create a list

			// Public or Private list
			switch($options["pubpriv"]) {
				case "public":	  $private = 0; break;
				case "private":	  $private = 1; break;
			}
			
			// TODO: We should be able to use the $_option variable
			//       array here some how (probably in a foreach()
			//       loop or something)
			$opts = (  ($options["archived"]		? "a"  : "")
					 . ($options["remotecfg"]		? "c"  : "")
					 . ($options["digest"]			? "d"  : "")
					 . ($options["prefix"]			? "f"  : "")
					 . ($options["guardedarchive"]	? "g"  : "")
					 . ($options["subhelp"]			? "h"  : "")
					 . ($options["indexed"]			? "i"  : "")
					 . ($options["subjump"]			? "j"  : "")
					 . ($options["kill"]			? "k"  : "")
					 . ($options["sublistable"]		? "l"  : "")
					 . ($options["moderated"]		? "m"  : "")
					 . ($options["modonly"]			? "o"  : "")
					 . ($private					? $p   : "")
					 . ($options["reqaddress"]		? "q"  : "")
					 . ($options["remoteadm"]		? "rn" : "")
					 . ($options["submoderated"]	? "s"  : "")
					 . ($options["trailers"]		? "t"  : "")
					 . ($options["subonly"]			? "u"  : "")
					 . ($options["extras"]			? "x"  : "")
					 . ($options["listparent"]		? " -0 ".$options["listparent"]  : "")
					 . ($options["fromaddress"]		? " -3 ".$options["fromaddress"] : "")
					 . ($options["listowner"]		? " -5 ".$options["listowner"]   : ""));
		} else {
			// Toggle a value
			// Change listowner, fromaddress or listparent - address is in $curropts
			switch($options) {
				case "listparent":	  $opts = "0 $curropts"; break;
				case "fromaddress":	  $opts = "3 $curropts"; break;
				case "owner":		  $opts = "5 $curropts"; break;
				default:
				  // A 'togglable' option
				  $opts = ($curropts[$options]
						   ? $this->_option[$options]["off"]
						   : $this->_option[$options]["on"]);
				  break;
			}
		}
// }}}

		if(is_numeric($listname)) {
			$listno   = $listname;

			$listpath = $this->mailing_lists[$listno]["directory"];
			$dotpath  = $this->mailing_lists[$listno]["dotpath"];
			$listname = $this->mailing_lists[$listno]["name"];
		} else {
			$listpath = $this->user_home . $listname;

			// Find the absolute path to the home directory of the user
			$line = @posix_getpwnam($this->user_name);
			if($line["dir"])
			  // A system user with a home directory
			  $dot_home = $line["dir"];
			else {
			  // {{{ Not a system user. Is it a qmail virtual user?
			  if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
				unset($out); unset($res);
				if($cmd = $this->_remote_list_setup("cat /var/qmail/users/assign")) {
				  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
					echo "CMD (updatelistentry): '$cmd'<br>";
				  
				  $ret = @exec($cmd, $out, $res);
				  if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
					printr($out);
				  
				  if(is_array($out) and !$res) {
					for($i=0; $i <= count($out); $i++) {
					  list($user, $alias, $uid, $gid, $dot_home, $x1, $fw, $x2) = $out[$i];
					  $user = preg_replace('/^=/', '',  $user);
					  if($user == $this->user_name)
						break;
					}
				  }
				}
			  } else {
				// Get the dot_home for 'this->user_name'.
				if(file_exists("/var/qmail/users/assign")) {
				  $fp = fopen("/var/qmail/users/assign", "r");
				  while($line = fgetcsv($fp, 1000, ":")) {
					list($user, $alias, $uid, $gid, $dot_home, $x1, $fw, $x2) = $line;
					$user = preg_replace('/^=/', '',  $user);
					if($user == $this->user_name)
					  // Found it, break out!
					  break;
				  }
				  fclose($fp);
				}
			  }
			  
			  if(empty($dot_home)) {
				// TODO: It's not a qmail virtual user either!
				$dot_home = '';
			  }
// }}}
			}

			if($dot_home) {
				if(! ereg("/$", $dot_home))
				  $dotpath  = $dot_home . "/.qmail-" . $listname;
				else
				  $dotpath  = $dot_home . ".qmail-" . $listname;
			} else
			  $dotpath  = "~alias/.qmail-" . $listname;
		}

		// {{{ Execute ezmlm-make
		$command = ($this->_paths["make"] . " 2>&1 -"
					. ($add		? "E "		: "+ ")
					. ($opts	? "-$opts"	: "")
					. " " . $listpath
					. " " . $dotpath
					. " " . $listname
					. " " . $domain);

		if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME")) {
			$output = do_exec($command, $out, $res);
			if($res != 0) {
				// Failed to create list
				$cmd = eregi_replace(" ",
									 " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
									 $command);

				$this->error = "Could not execute system command.<br><font size=-4>"
				  . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
				  . "</font><br>Error returned: "
				  . (!strcmp($output, "") ? "" : " " . $output . "</b>");
				return false;
			} else
			  // Successfully created list
			  $this->_chmodown($listpath, $this->user_name);
		} else
		  echo "Command:<br><pre>$command</pre><br>";
// }}}

		// Update '$this->mailing_lists' array so we don't have to re-read!
		$list_index = count($this->mailing_lists);
		$this->mailing_lists[$list_index]["directory"]	= $listpath;
		$this->mailing_lists[$list_index]["dotpath"]	= $dotpath;
		$this->mailing_lists[$list_index]["name"]		= $listname;
		$this->mailing_lists[$list_index]["host"]		= $domain;

		return true;
	}
// }}}

	// {{{ deletelistentry(listnumber)
	// Delete a mailinglist
	function deletelistentry($listnumber) {
		$lstname = $this->mailing_lists[$listnumber]["name"];
		$lstpath = $this->mailing_lists[$listnumber]["directory"];
		$domain  = $this->mailing_lists[$listnumber]["host"];

		$dotpath = dirname( $this->mailing_lists[$listnumber]["dotpath"]);
		$dotfile = basename($this->mailing_lists[$listnumber]["dotpath"]);

		// {{{ Delete all files and directories recursively
		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);

		  if($this->debug or file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$command = "echo rm -R $lstpath";
		  else
			$command = "echo rm -R $lstpath";

		  if($cmd = $this->_remote_list_setup($command)) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (deletelistentry/1): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);

			if($res)
			  $unsuccessful = 1;
		  }
		} else {
		  $files = $this->_getfsentries($lstpath);
		  $files[] = $lstpath;
		  for($i=0; $i < count($files); $i++) {
			if(is_dir($files[$i])) {
				// This is a DIRECTORY entry
				if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME")) {
					if(! rmdir($files[$i]) ) {
						echo "Could not delete dir $files[$i]<br>";
						$unsuccessful = 1;
					}
				} else
				  echo "DEBUG: deleting <b>dir</b> : $files[$i]<br>";
			} else {
				// This is a FILE entry
				if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME")) {
					if(! unlink($files[$i]) ) {
						echo "Could not delete file $files[$i]<br>";
						$unsuccessful = 1;
					}
				} else
				  echo "DEBUG: deleting file: $files[$i]<br>";
			}
		  }

		  if($this->debug)
			echo "#Entries: $i<br>";
		}
		// }}}

		// {{{ Now time for dot files in ~alias/.qmail-LISTNAME
		$links = $this->_getdotlinks($dotpath, $dotfile);
		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);

		  if($this->debug or file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$command = "echo rm ";
		  else
			$command = "echo rm ";

		  for($i=0; $i <= count($links); $i++)
			$command .= $links[$i];

		  if($cmd = $this->_remote_list_setup($command)) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (deletelistentry/2): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);

			if($res)
			  $unsuccessful = 1;
		  }
		} else {
		  for($i=0; $i < count($links); $i++) {
			if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME")) {
				if(! unlink($links[$i]) ) {
					echo "Could not delete link $links[$i]<br>";
					$unsuccessful = 1;
				}
			} else
			  echo "DEBUG: deleting link: $links[$i]<br>";
		  }
		  
		  if($this->debug)
			echo "#Entries: $i<br>";
		}
		// }}}

		if($unsuccessful) {
			$this->error = "Could not delete mailinglist $lstname from directory $lstpath";
			return false;
		}

		return true;
	}
// }}}

	// {{{ savelistentrytexts(listname, text, contents)
	// Create/Modify list text
	function savelistentrytexts($listname, $text, $contents) {
		// Create a temp file
		if(! strcmp($list_path = tempnam($this->user_home."/".$listname."/text", $text.'.'), "")) {
			$this->error = "could not create a temporary the mailing list text file name";
			return false;
		}

		// Open the temp file for write
		if( ($file = @fopen($list_path, "w")) !=0 ) {
			$this->error = "could not create a temporary the mailing list text file";
			return false;
		}
		
		// Write text content to temp file
		if( fwrite($file, $contents, strlen($contents)) !=0) {
			fclose($file);
			unlink($list_path);

			$this->error = "could not write to a temporary the mailing list text file";
			return false;
		}
		fclose($file);

		// Move the tempfile to the correct dir/filename
		$dest = ($this->user_home."/".$listname."/text/".$text);
		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);
		  if($cmd = $this->_remote_list_setup("rcp $list_path $dest")) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (savelistentrytexts): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
		  }
		} else
		  $success = copy($list_path, $dest);

		unlink($list_path);
		return $success;
	}
// }}}

	// {{{ subscribe(listname, subscriber)
	// Add subscriber
	function subscribe($listname, $subscriber) {
		if(is_numeric($listname)) {
			$listno		= $listname;

			$domain     = $this->mailing_lists[$listname]["host"];
			$listname	= $this->mailing_lists[$listname]["name"];

			if(!ereg("/$", $this->mailing_lists[$listname]["directory"]))
			  $listpath	= $this->mailing_lists[$listname]["directory"] . "/" . $listname;
			else
			  $listpath	= $this->mailing_lists[$listname]["directory"] . $listname;
		} elseif(!empty($this->mailing_lists)) {
			$domain     = $this->mailing_lists[0]["host"];

			if(!ereg("/$", $this->user_home))
			  $listpath	= $this->user_home . "/" . $listname;
			else
			  $listpath	= $this->user_home . $listname;
		} // TODO: else!?

		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);

		  // {{{ Execute ezmlm-sub
		  $command = $this->_remote_list_setup($this->_paths["sub"] . " $listpath $subscriber");

		  if(!$this->debug or !file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$output = do_exec($command, $out, $res);
		  else {
			echo "Command:<br><pre>$command</pre><br>";
			$res = 0;
		  }
		  // }}}
		} else {
		  // {{{ Execute ezmlm-sub
		  $command = $this->_remote_list_setup($this->_paths["sub"] . " 2>&1 $listpath $subscriber");

		  if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$output = do_exec($command, $out, $res);
		  else {
			echo "Command:<br><pre>$command</pre><br>";
			$res = 0;
		  }
		  // }}}
		}

		if($res) {
		  // Failed to subscribe user
		  $cmd = eregi_replace(" ",
							   " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
							   $command);
		  
		  $this->error = "Could not execute system command.<br><font size=-4>"
			. "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
			. "</font><br>Error returned: "
			. (!strcmp($output, "") ? "" : " " . $output . "</b>");
		  return false;
		}

		return true;
	}
// }}}

	// {{{ unsubscribe(listname, subscriber)
	// Remove subscriber
	function unsubscribe($listname, $subscriber) {
		if(is_numeric($listname)) {
			$listno   = $listname;

			$domain   = $this->mailing_lists[$listno]["host"];
			$listpath = $this->mailing_lists[$listno]["directory"];
			$listname = $this->mailing_lists[$listno]["name"];
		} else {
			$domain   = $this->mailing_lists[0]["host"];
			$listpath = $this->user_home . "/" . $listname;
		}

		if(pql_get_define("PQL_CONF_EZMLM_REMOTE")) {
		  unset($out); unset($res);

		  $command = $this->_paths["sub"] . " $listpath $subscriber";
		  if($this->debug or file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$command = "echo $command";

		  if($cmd = $this->_remote_list_setup($command)) {
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  echo "CMD (subscribe): '$cmd'<br>";
			
			$ret = @exec($cmd, $out, $res);
			if(file_exists($_SESSION["path"]."/.DEBUG_ME"))
			  printr($out);
		  }
		} else {
		  $command = $this->_paths["unsub"] . " 2>&1 " . $listpath . " " . $subscriber;
		  if(!$this->debug and !file_exists($_SESSION["path"]."/.DEBUG_ME"))
			$output = do_exec($command, $out, $res);
		  else
			echo "Command:<br><pre>$command</pre><br>";
		}

		if($res) {
		  // Failed to unsubscribe user
		  $cmd = eregi_replace(" ",
							   " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
							   $command);
		  
		  $this->error = "Could not execute system command.<br><font size=-4>"
			. "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
			. "</font><br>Error returned: "
			. (!strcmp($output, "") ? "" : " " . $output . "</b>");
		  return false;
		}

		return true;
	}
// }}}

	// {{{ subscribe_kill(listname, address)
	function subscribe_kill($listname, $address) {
		$this->subscribe($listname."/deny", $address);
	}
// }}}
 
	// {{{ unsubscribe_kill(listname, address)
	function unsubscribe_kill($listname, $address) {
		$this->unsubscribe($listname."/deny", $address);
	}
// }}}
// }}}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * folded-file: t
 * End:
 */
?>
