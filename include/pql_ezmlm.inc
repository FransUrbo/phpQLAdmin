<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ezmlm.inc,v 1.26 2004-10-19 10:40:42 turbo Exp $
session_start();

class ezmlm {
	// ===================================================
	// {{{ Private section
	var $_option					= array();
	var $_paths						= array();

	// {{{ _getfsentries(dir)

	// Get filesystem entries (files or directories) recursively.
	function _getfsentries($dir) {
		if(!(($directory = @opendir($dir)) !=0)) {
			$this->error = "could not open the directory '$dir'";
			return 0;
		}
		
		for(;GetType($entry = readdir($directory)) == "string";) {
			if($entry == "." || $entry == "..")
			  continue;
			
			if(@is_dir($dir."/".$entry)) {
				// directory - recursive invocation
				
				$e = $this->_getfsentries($dir . "/" . $entry);
				for($i=0; $e[$i]; $i++) {
					$entries[] = $e[$i];
				}
			}

			// First the file above, then the directory.
			// That way we get it sorted and done for
			// deletion!
			$entries[] = $dir . "/" . $entry;
		}
		closedir($directory);

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

		if(!(($directory = @opendir($dir)) !=0)) {
			$this->error = "could not open the directory '$dir'";
			return 0;
		}
		
		for(;GetType($entry = readdir($directory)) == "string";) {
			if($entry == "." || $entry == "..")
			  continue;

			$target = $dir . "/" . $entry;
			if(@is_link($target)) {
				// It's a link.

				for($i=0; $regexp[$i]; $i++) {
					$lookfor = $dir.'.*/'.$base.$regexp[$i];

					if(ereg($lookfor, $target)) {
						$entries[] = $target;
					}
				}
			}
		}
		closedir($directory);

		return $entries;
	}
	// }}}

	// {{{ _readlists(loadtexts)
	// load list of mailinglists in directory
	function _readlists($loadtexts = 0) {
		if(!(($directory = @opendir($this->user_home)) !=0)) {
			$this->error="could not open the ezmlm user home directory";
			return 0;
		}

		for(;GetType($entry_name = readdir($directory)) == "string";) {
			if((strcmp(substr($entry_name, 0, 1),".") &&
				@is_dir($this->user_home."/".$entry_name) &&
				file_exists($this->user_home."/".$entry_name."/config")))
			  {
				  $list_index = Count($this->mailing_lists);
				  if(!$this->_readlistconfiguration($entry_name,
													$this->mailing_lists[$list_index],
													$loadtexts))
					{
						closedir($directory);
						return 0;
					}

				  $this->mailing_lists_index[$entry_name] = $list_index;
			  }
		}
		closedir($directory);

		ksort($this->mailing_lists_index);
		foreach($this->mailing_lists_index as $name => $no) {
			$host = $this->mailing_lists[$no]["host"];
			$this->mailing_lists_hostsindex[$host][$name] = $no;
		}

		$this->mailing_lists_hostsindex["COUNT"] = count($this->mailing_lists_hostsindex[$host]);
		return 1;
	}
	// }}}

	// {{{ _readlistconfiguration(list, configuration, loadtexts)
	// Read list configuration
	function _readlistconfiguration($list, &$configuration, $loadtexts) {
		if(! ereg("/$", $user_home))
		  $list_path = ($this->user_home.$list);
		else
		  $list_path = ($this->user_home."/$list");
		if(!( ($file = @fopen($list_path."/config", "r")) !=0)) {
			$this->error="could not open the mailing list configuration file";
			return 0;
		}

		$configuration["name"] = $list;

		for(;!feof($file);) {
			if(!(GetType($line = fgets($file, 256)) == "string")) {
				if(feof($file)) {
					break;
				}

				fclose($file);
				$this->error="could not read the mailing list configuration file";
				return 0;
			}

			if( (GetType($endofline = strpos($line, "\n")) == "integer") )
			  $line = substr($line, 0, $endofline);

			if( (GetType($endofline = strpos($line, "\r")) == "integer") )
			  $line = substr($line, 0, $endofline);

			if( (!GetType($colon = strpos($line, ":")) == "integer" || $colon == 0) ) {
				fclose($file);
				$this->error = "invalid mailing list configuration file";
				return 0;
			}

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
		fclose($file);

		// ------------------------
		// Get subscriber addresses
		$command = $this->_paths["list"] . " 2>&1 " . $list_path;
		$output = @Exec($command, $output_array, $result);
		if($result != 0) {
			$this->error = "could not execute system command: '" . $command . "'" .
			  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
			return 0;
		}

		$configuration["subscribers"] = count($output_array);
		for($i=0; $output_array[$i]; $i++)
		  $configuration["subscriber"][] = $output_array[$i];

		// ------------------------
		if(file_exists($list_path . "/deny/lock")) {
			// Get deny addresses
			unset($output_array); unset($result);

			$command = $this->_paths["list"] . " 2>&1 " . $list_path . "/deny";
			$output = @Exec($command, $output_array, $result);
			if($result != 0) {
				$this->error = "could not execute system command: '" . $command . "'" .
				  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
				return 0;
			}

			$configuration["killlisted"] = count($output_array);
			for($i=0; $output_array[$i]; $i++)
			  $configuration["killlist"][] = $output_array[$i];
		}

		// ------------------------
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

		return 1;
	}
	// }}}

	// {{{ _verifysubscriber(list, subscriber)
	// Verify subscriber (is the address subscribed?)
	function _verifysubscriber($list, $subscriber) {
		$list_path = $this->user_home."/".$list;
		$command   = $this->_paths["list"] . " 2>&1 ".$list_path;
		$output    = do_exec($command, $output_array, $result);
		if($result != 0) {
			$this->error = "Could not execute system command: ".$command
			  . ((!strcmp($output,"")) ? "" : (" (".$output.")"));
			return 0;
		}

		$subscriber_address = strtolower($subscriber);
		$address = 0;
		$addresses = Count($output_array);
		for(; $address < $addresses;) {
			if(! strcmp($subscriber_address, strtolower($output_array[$address]))) {
				return 1;
			}
			$address++;
		}

		return 0;
	}
	// }}}

	// {{{ _chmodown(directory, username, dirmode, filemode)
	// Chown and Chmod directory recursive
	function _chmodown($directory, $username, $dirmode = '0770', $filemode = '0660') {
		// Find user id for the username
		$uidnr = @posix_getpwnam($username);
		$uidnr = $uidnr["uid"];

		umask(0011);

		chmod($directory, octdec($dirmode));

		$files = $this->_getfsentries($directory);
		for($i=0; $files[$i]; $i++) {
			if(is_dir($files[$i])) {
				chmod($files[$i], octdec($dirmode));
			} else {
				chmod($files[$i], octdec($filemode));
			}

			// TODO: We don't have access to do this!!!!
			//chown($files[$i], $uidnr);
		}

//		putenv("PQL_USERGROUP=$uidnr.".posix_getuid());
//		putenv("PQL_DIRECTORY=".$directory);
//		pql_execute('./sub_ezmlm_create.pl', 1);
	}
	// }}}
	// }}}

	// ===================================================
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
	function ezmlm($user_name = 'newsletters', $user_home = '/home/newsletters', $paths = 0) {
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
		if(! $paths) {
			$this->_paths["make"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-make";
			$this->_paths["list"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-list";
			$this->_paths["sub"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-sub";
			$this->_paths["unsub"]	= pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-unsub";
		} else {
			// Assign EXISTING values
			foreach($paths as $key => $val) {
				$this->_paths[$key] = $paths[$key];
			}

			// Assign EXISTING values with defaults
			$keys = array('make', 'list', 'sub', 'unsub');
			foreach($keys as $key) {
				if(! $this->_paths[$key]) {
					$this->_paths[$key] = pql_get_define("PQL_CONF_EZMLM_PATH") . "/ezmlm-" . $key;
				}
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
	function updatelistentry($add, $listname, $domain, $options, $curropts = 0) {
		// * If we should create a list, set $add to 1/TRUE
		//   and $options should contain the options to enable.
		//
		// * To modify (ie, toggle) a value on an existing list,
		//   set $add to 0/FALSE, $options to the attribute to
		//   change and $curropts to the current list options

		if($add) {
			// Create a list

			// Public or Private list
			if($options["pubpriv"]) {
				if($options["pubpriv"] == 'public') {
					$private = 0;
				} elseif($options["pubpriv"] == 'private') {
					$private = 1;
				}
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
					 . ($options["listparent"]	? " -0 ".$options["listparent"]  : "")
					 . ($options["fromaddress"]	? " -3 ".$options["fromaddress"] : "")
					 . ($options["listowner"]	? " -5 ".$options["listowner"]   : ""));
		} else {
			// Toggle a value
			// Change listowner, fromaddress or listparent - address is in $curropts
			if($options == 'listparent') {
				$opts = "0 $curropts";
			} elseif($options == 'fromaddress') {
				$opts = "3 $curropts";
			} elseif($options == 'owner') {
				$opts = "5 $curropts";
			} else {
				// A 'togglable' option
				$opts = ($curropts[$options]
						 ? $this->_option[$options]["off"]
						 : $this->_option[$options]["on"]);
			}
		}

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
				// Not a system user. Is it a qmail virtual user?
				$fp = fopen("/var/qmail/users/assign", "r");
				while($line = fgetcsv($fp, 1000, ":")) {
					list($user, $alias, $uid, $gid, $dot_home, $x1, $fw, $x2) = $line;
					$user = preg_replace('/^=/', '',  $user);
					if($user == $this->user_name)
					  break;
				}
				fclose($fp);

//				if(!$dot_home) {
//					// TODO: It's not a qmail virtual user either!
//				}
			}

			if($dot_home) {
				if(! ereg("/$", $dot_home))
				  $dotpath  = $dot_home . "/.qmail-" . $listname;
				else
				  $dotpath  = $dot_home . ".qmail-" . $listname;
			} else
			  $dotpath  = "~alias/.qmail-" . $listname;
		}

		$command = ($this->_paths["make"] . " 2>&1 -"
					. ($add		? "E "		: "+ ")
					. ($opts	? "-$opts"	: "")
					. " " . $listpath
					. " " . $dotpath
					. " " . $listname
					. " " . $domain);

		if(!$this->debug and !file_exists("./.DEBUG_ME")) {
			$output = do_exec($command, $output_array, $result);
			if($result != 0) {
				// Failed to create list
				$cmd = eregi_replace(" ",
									 " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
									 $command);

				$this->error = "Could not execute system command.<br><font size=-4>"
				  . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
				  . "</font><br>Error returned: "
				  . (!strcmp($output, "") ? "" : " " . $output . "</b>");
				return 0;
			} else
			  // Successfully created list
			  $this->_chmodown($listpath, $this->user_name);
		} else
		  echo "Command:<br><pre>$command</pre><br>";

		return 1;
	}
	// }}}

	// {{{ deletelistentry(listnumber)

	// Delete a mailinglist
	function deletelistentry($listnumber) {
		$lstname = $this->mailing_lists[$listnumber]["name"];
		$lstpath = $this->mailing_lists[$listnumber]["directory"];
		$domain  = $this->mailing_lists[$listnumber]["host"];

		$dotpath = dirname($this->mailing_lists[$listnumber]["dotpath"]);
		$dotfile = basename($this->mailing_lists[$listnumber]["dotpath"]);

		// Delete all files and directories recursively
		$files = $this->_getfsentries($lstpath);
		$files[] = $lstpath;
		for($i=0; $files[$i]; $i++) {
			if(is_dir($files[$i])) {
				if(!$this->debug and !file_exists("./.DEBUG_ME")) {
					if(! rmdir($files[$i]) ) {
						echo "Could not delete dir $files[$i]<br>";
						$unsuccessful = 1;
					}
				} else {
					echo "DEBUG: deleting <b>dir</b> : $files[$i]<br>";
				}
			} else {
				if(!$this->debug and !file_exists("./.DEBUG_ME")) {
					if(! unlink($files[$i]) ) {
						echo "Could not delete file $files[$i]<br>";
						$unsuccessful = 1;
					}
				} else {
					echo "DEBUG: deleting file: $files[$i]<br>";
				}
			}
		}
		if($this->debug) {
			echo "#Entries: $i<br>";
		}

		// Now time for dot files in ~alias/.qmail-LISTNAME
		$links = $this->_getdotlinks($dotpath, $dotfile);
		for($i=0; $links[$i]; $i++) {
			if(!$this->debug and !file_exists("./.DEBUG_ME")) {
				if(! unlink($links[$i]) ) {
					echo "Could not delete link $links[$i]<br>";
					$unsuccessful = 1;
				}
			} else {
				echo "DEBUG: deleting link: $links[$i]<br>";
			}
		}
		if($this->debug) {
			echo "#Entries: $i<br>";
		}

		if($unsuccessful) {
			$this->error = "Could not delete mailinglist $lstname from directory $lstpath";
			return 0;
		} else {
			return 1;
		}
	}

	// }}}

	// {{{ savelistentrytexts(listname, text, contents)
	// Create/Modify list text
	function savelistentrytexts($listname, $text, $contents) {
		// Create a temp file
		if(! strcmp($path = tempnam($this->user_home."/".$listname."/text", $text.'.'), "")) {
			$this->error = "could not create a temporary the mailing list text file name";
			return 0;
		}

		// Open the temp file for write
		if( ($file = @fopen($path, "w")) !=0 ) {
			$this->error = "could not create a temporary the mailing list text file";
			return 0;
		}
		
		// Write text content to temp file
		if( fwrite($file, $contents, strlen($contents)) !=0) {
			fclose($file);
			unlink($path);

			$this->error = "could not write to a temporary the mailing list text file";
			return 0;
		}
		fclose($file);

		// Move the tempfile to the correct dir/filename
		$success = copy($path, ($this->user_home."/".$listname."/text/".$text));
		unlink($path);

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
			$listpath	= $this->mailing_lists[$listname]["directory"] . "/" . $listname;
		} else {
			$domain     = $this->mailing_lists[0]["host"];
			$listpath	= $this->user_home . "/" . $listname;
		}

		$command		= $this->_paths["sub"] . " 2>&1 " . $listpath . " " . $subscriber;
		if(!$this->debug and !file_exists("./.DEBUG_ME")) {
			$output    = do_exec($command, $output_array, $result);
			if($result != 0) {
				// Failed to subscribe user
				$cmd = eregi_replace(" ",
									 " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
									 $command);

				$this->error = "Could not execute system command.<br><font size=-4>"
				  . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
				  . "</font><br>Error returned: "
				  . (!strcmp($output, "") ? "" : " " . $output . "</b>");
				return 0;
			}
		} else
		  echo "Command:<br><pre>$command</pre><br>";

		return 1;
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

		$command		= $this->_paths["unsub"] . " 2>&1 " . $listpath . " " . $subscriber;
		if(!$this->debug and !file_exists("./.DEBUG_ME")) {
			$output    = do_exec($command, $output_array, $result);
			if($result != 0) {
				// Failed to unsubscribe user
				$cmd = eregi_replace(" ",
									 " \\<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;",
									 $command);

				$this->error = "Could not execute system command.<br><font size=-4>"
				  . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . $cmd
				  . "</font><br>Error returned: "
				  . (!strcmp($output, "") ? "" : " " . $output . "</b>");
				return 0;
			}
		} else
		  echo "Command:<br><pre>$command</pre><br>";

		return 1;
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
