<?php
// attribute plugin for
// accountstatus (status of mailaccount)
// $Id: attrib.accountstatus.inc,v 2.34 2007-09-29 21:15:11 turbo Exp $

/* DLW:   When the calendar gets a value of 2003.12.25, it gets stored or displayed as 2003/12/24 (@7am)
 *        I think it's having problems with time zones.  Look into it.
 *
 * TURBO: It's not a problem with THIS file. I'm swede, so I'm used to the date format
 *        '[YY]YYMMHH'. So I'm setting the JSCalendar widget to output such a value.
 *        However, when DISPLAYING the value stored, phpQLAdmin must honor the time zone(s).
 *
 * The values created by the JSCalendar is used in two (?) places:
 * - [user]->Account status->Account expiration date
 * - [user]->Account status->Account deletion date   (outputs 'Feb 28, 2005' for example).
 */

// {{{ attribute_check(type)
/* attribute_check is sometimes called directly (from attribute_print_form)
 * so I use _REQUEST variables instead of _POST variables. */
function attribute_check($type) {
    global $LANG;
    
	if($type != "modify") {
		switch($_REQUEST["set"]) {
		  case "active":
		  case "nopop":
		  case "noaccess":
		  case "disabled":
		  case "deleted":
		  case "expiration":
			return true;
			
		  default:
			$_REQUEST["set"] = pql_complete_constant($LANG->_('Unknown attribute value %value% for %attribute%'),
													 array('value' => $set, 'attribute' => $LANG->_('accountstatus')));
			return false;
		}
	} else {
		$year = substr($_REQUEST["date"], 0, 4);
		$month = substr($_REQUEST["date"], 4, 2);
		$day = substr($_REQUEST["date"], 6, 2);

		// Convert the YYYY.MM.DD date to UNIX std time (sec from 1 Jan 1970)
		$_REQUEST["date"] = date("U", mktime(0,0,0,$month,$day,$year));
		// Get 'days from Jan 1, 1970' which is the correct integer for shadowExpire
		$_REQUEST["days"] = explode('\.', $_REQUEST["date"] / (60 * 60 * 24));
		$_REQUEST["days"] = $_REQUEST["days"][0];

		if($_REQUEST["days"]) {
			return true;
		} else {
			return false;
		}
	}
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    // dummy function
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "set") {
    global $LANG;

    if(($_REQUEST["set"] != 'expiration') and ($_REQUEST["set"] != 'deleted')) {
		// No form neccessary - save directly

		if(!attribute_check($type)) {
			$msg = $_REQUEST["set"];
			attribute_forward($msg);
		}

		attribute_save($type);
	} else {
	  // Output a form requesting date when account expires or if marking
	  // an account deleted, ask for 'qmailAccountPurge'.
	  // Java script etc taken from http://dynarch.com/mishoo/calendar.epl
?>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3">
      <th colspan="3" align="left">
<?php if($_REQUEST["set"] == 'deleted') { ?>
        <?=$LANG->_('Time when the account should be deleted')?></th>
<?php } else { ?>
        <?=$LANG->_('Time when the account is disabled')?></th>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
<?php if($_REQUEST["set"] == 'deleted') { ?>
          <td class="title"><?=$LANG->_('Deletion date')?></td>
<?php } else { ?>
          <td class="title"><?=$LANG->_('Expiration date')?></td>
<?php } ?>
          <td><input type="text" name="date" id="f_date_c" readonly="1" /></td>
          <td><img src="jscalendar/img.gif" id="f_trigger_c" style="cursor: pointer; border: 1px solid red;" title="Date selector"
                   onmouseover="this.style.background='red';" onmouseout="this.style.background=''" /></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
<?php if($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_ISACTIVE"))) { ?>
    <input type="hidden" name="submit"   value="5">
    <input type="hidden" name="action"   value="set">
    <input type="hidden" name="set"      value="<?=$_REQUEST["set"]?>">
<?php } else { ?>
    <input type="hidden" name="submit"   value="1">
<?php } ?>
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

  <script type="text/javascript">
      Calendar.setup({
          inputField     :    "f_date_c",     // id of the input field
          ifFormat       :    "%Y%m%d",       // format of the input field (epoc='%s')
          button         :    "f_trigger_c",  // trigger for the calendar (button ID)
          align          :    "Bl",           // alignment (Vertical: top, center, bottom. Horizontal: left, center, right)
          singleClick    :    true
      });
  </script>
<?php
	}
}
// }}}

// {{{ attribute_save(type)
/* attribute_save is sometimes called directly (from attribute_print_form)
 * so I use _REQUEST variables instead of _POST variables. */
function attribute_save($type) {
    global $_pql, $LANG;

    switch($type) {
      case "fulldomain":
      case "modify":
		// {{{ Modify or Fulldomain
		if($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_EXPIRATION"))) {
			// Set the shadowExpire attribute for this user
			
			// Get the objectClasses of this user
			$entry = $_pql->get_attribute($_REQUEST["user"], 'objectclass');

			// Make sure that the user have the 'shadowAccount' objectclass
			for($i=0; $i < count($entry); $i++) {
				$new_entry["objectclass"][] = $entry[$i];

				if(lc($entry[$i]) == lc('shadowAccount') )
				  $got_objectclass = 1;
			}

			// Add the objectclass if it doesn't already exists
			if(!$got_objectclass)
			  $new_entry["objectclass"][] = "shadowAccount";

			// Add the attribute and it's value
			$new_entry[pql_get_define("PQL_ATTR_EXPIRATION")] = $_REQUEST["days"];

			// Modify the object
			if(!$_pql->modify($_REQUEST["user"], $new_entry, "attrib.accountstatus.inc:attribute_save()/1"))
			  $msg  = $LANG->_('Failed to set account expiration date') . ": " . pql_format_error(0);
			else
			  $msg = $LANG->_('Successfully set account expiration date');
		} else {
			// Create a user search filter
			if(pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]))
			  $filter = pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]);
			else
			  $filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
			
			// Retreive all users in this branch/domain.
			$users   = $_pql->get_dn($_REQUEST["domain"], $filter);
			if(is_array($users)) {
				foreach($users as $user)
				  if(!pql_modify_attribute($user, pql_get_define("PQL_ATTR_ISACTIVE"),
										   '', $_REQUEST["set"])) {
					  $msg  = pql_complete_constant($LANG->_('Failed to change %what%'),
													array('what' => $LANG->_('Status')));
					  $msg .= ": " . pql_format_error(0);
				  } else
					$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
												 array('what' => $LANG->_('Status'),
													   'new'  => pql_ldap_accountstatus($_REQUEST["set"])));
			}
		}

		attribute_forward($msg);
		break;
		// }}}
		
      case "set":
		// {{{ set
		if(pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_ISACTIVE"),
								1, $_REQUEST["set"])) {
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('Status'),
											   'new'  => pql_ldap_accountstatus($_REQUEST["set"])));

			if($_REQUEST["attrib"] == sprintf("%s", pql_get_define("PQL_ATTR_ISACTIVE"))) {
			  if($_REQUEST["set"] == "deleted") {
				// Convert YYYYMMDD to epoc
				$epoc = pql_format_timestamp_unixtime($_REQUEST["date"]);
				
				pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_ACCOUNT_PURGE"), 1,$epoc);
			  } else {
				// Everything exept deletion - if we have a qmailAccountPurge attribute, this MUST be deleted.
				$date = $_pql->get_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_ACCOUNT_PURGE"));
				if($date)
				  pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_ACCOUNT_PURGE"), 1, '');
			  }
			}
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Status'))) . ": " . pql_format_error(0);
		
		if(pql_get_define("PQL_CONF_DEBUG_ME"))
		  die($msg);
		else
		  attribute_forward($msg);
		break;
		// }}}
		
      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
