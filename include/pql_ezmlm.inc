<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ezmlm.inc,v 1.6 2002-12-25 01:23:50 turbo Exp $
class ezmlm {
	// ===================================================
	// Private section
	var $_option   = array();
	var $srch_path = '';
	var $user_name = '';
	var $user_home = '';

	// Read list configuration
	function _readlistconfiguration($list, &$configuration, $loadtexts) {
		$list_path = ($this->user_home."/".$list);
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

		// Get number of subscribers
		$command = $this->srch_path . "/ezmlm-list 2>&1 " . $list_path . " | wc -l";
		$output  = @Exec($command, $output_array, $result);
		if($result != 0) {
			$this->error = "could not execute system command: '" . $command . "'" .
			  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
			return 0;
		}
		$configuration["subscribers"] = intval($output);

		// Get subscriber addresses
		$command = $this->srch_path . "/ezmlm-list 2>&1 " . $list_path;
		$output = @Exec($command, $output_array, $result);
		if($result != 0) {
			$this->error = "could not execute system command: '" . $command . "'" .
			  ((!strcmp($output, "")) ? "" : (" (".$output.")"));
			return 0;
		}
		for($i=1; $output_array[$i]; $i++) {
			$configuration["subscriber"][] = $output_array[$i];
		}

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

	// Verify subscriber (is the address subscribed?)
	function _verifysubscriber($list, $subscriber) {
		$list_path = $this->user_home."/".$list;
		$command   = $this->srch_path."/ezmlm-list 2>&1 ".$list_path;
		$output    = @Exec($command, $output_array, $result);
		if($result != 0) {
			$this->error = "could not execute system command: ".$command
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

	// ===================================================
	// Public section
	var $error_message				= "";
	var $error						= "";
	var $debug						= 1;

	var $mailing_lists				= array();
	var $mailing_lists_index		= array();
	var $mailing_lists_hostindex	= array();

	// Constructor. Intialize the variable array of
	// attribute and what command line option to ezmlm-make
	// this corresponds to when on or off.
	// Couldn't do this 'globaly', at variable declaration
	// above, so I had to do it in the costructor. I don't
	// have a better use for it anyway :)
	function ezmlm($srch_path = '/usr/bin', $user_name = 'newsletters', $user_home = '/home/newsletters') {
		$this->_option["archived"]["on"]		= "a";	$this->_option["archived"]["off"]		= "A";
		$this->_option["remotecfg"]["on"]		= "c";	$this->_option["remotecfg"]["off"]		= "C";
		$this->_option["digest"]["on"]			= "d";	$this->_option["digest"]["off"]			= "D";
		$this->_option["prefix"]["on"]			= "f";	$this->_option["prefix"]["off"]			= "F";
		$this->_option["guardedarchive"]["on"]	= "g";	$this->_option["guardedarchive"]["off"]	= "G";
		$this->_option["subhelp"]["on"]			= "h";	$this->_option["subhelp"]["off"]		= "H";
		$this->_option["indexed"]["on"]			= "i";	$this->_option["indexed"]["off"]		= "I";
		$this->_option["subjump"]["on"]			= "j";	$this->_option["subjump"]["off"]		= "J";
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

		$this->srch_path = $srch_path;
		$this->user_name = $user_name;
		$this->user_home = $user_home;
	}

	// Load list of mailinglists in directory
	function readlists($loadtexts = 0) {
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

		return 1;
	}

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

		$command = ($this->srch_path."/ezmlm-make 2>&1 -"
					. ($add		? "E"		: "+ ")
					. ($opts	? "-$opts"	: "")
					. " "
					. $this->user_home."/".$listname
					. " "
					. "~".$this->user_name."/.qmail-".$listname
					. " "
					. $listname
					. " "
					. $domain);

		if(! $this->debug) {
			$output = @Exec($command, $output_array, $result);
			if($result != 0) {
				$this->error = "could not execute system command: " . $command 
				  . (!strcmp($output, "") ? "" : " (" . $output . ")");
				return 0;
			}
		} else {
			die("Command: <br><pre>$command</pre><br>");
		}

		return 1;
	}

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

	// Add subscriber
	function subscribe($list, $subscriber) {
		$list_path = $this->user_home."/".$list;
		$command   = $this->srch_path."/ezmlm-sub 2>&1 ".$list_path." ".$subscriber;
		if(! $this->debug) {
			$output    = @Exec($command, $output_array, $result);
			if($result != 0) {
				$this->error = "could not execute system command: ".$command
				  . ((!strcmp($output,"")) ? "" : (" (".$output.")"));
				return 0;
			}
		} else {
			die("Command: <br><pre>$command</pre><br>");
		}

		return 1;
	}
 
	// Remove subscriber
	function unsubscribe($list, $subscriber) {
		$list_path = $this->user_home."/".$list;
		$command   = $this->srch_path."/ezmlm-unsub 2>&1 ".$list_path." ".$subscriber;
		if(! $this->debug) {
			$output    = @Exec($command, $output_array, $result);
			if($result != 0) {
				$this->error = "could not execute system command: ".$command
				  . ((!strcmp($output,"")) ? "" : (" (".$output.")"));
				return 0;
			}
		} else {
			die("Command: <br><pre>$command</pre><br>");
		}

		return 1;
	}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
