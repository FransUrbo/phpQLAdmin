<?php
// ----------------------------
// pql_ezmlm.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_ezmlm.inc,v 1.3 2002-12-22 20:28:05 turbo Exp $
class ezmlm {
	// Public variables
	var $error_message				= "";
	var $error						= "";
	var $mailing_lists				= array();
	var $mailing_lists_index		= array();
	var $mailing_lists_hostindex	= array();

	var $srch_path = '/usr/bin';
	var $user_name = 'newsletters';
	var $user_home = '/home/newsletters';

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
				if(!$this->readlistconfiguration($entry_name,
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

	function updatelistentry($add) {
		$command = ($this->srch_path."/ezmlm-make 2>&1 -".(($add) ? "E" : "e").(($this->edit_list_form_object->GetCheckedState("moderated")) ? "m" : "M").(($this->edit_list_form_object->GetCheckedState("archived")) ? "a" : "A")." -5 ".$this->edit_list_form_object->GetInputValue("list_owner")." ".$this->ezmlm_user_home."/".$this->list_name." ".$this->ezmlm_user_home."/.qmail-".$this->list_name." ".$this->list_name." ".$this->edit_list_form_object->GetInputValue("list_host"));
		echo "Command: '$command'<br>";
		die();
		//  $output=@Exec($command,$output_array,$result);
		if(($result!=0)) {
			$this->error=("could not execute system command: ".$command.((!strcmp($output,"")) ? "" : (" (".$output.")")));
			return 0;
		}
		return 1;
	}

	function readlistconfiguration($list, &$configuration, $loadtexts) {
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
			  case "5":
				$configuration["owner"] = $value;
				break;

			  case "F":
				$configuration["moderated"] = GetType(strpos($value, "m")) == "integer";
				$configuration["archived"]  = GetType(strpos($value, "a")) == "integer";
				break;

			  case "H":
				$configuration["host"] = $value;
				break;

			  case "L":
				$configuration["local"] = $value;
				break;
			}
		}
		fclose($file);

		// Get number of subscribers
		$command = $this->srch_path . "/ezmlm-list 2>&1 " . $list_path . " | wc -l";
		$output = @Exec($command, $output_array, $result);
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
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
