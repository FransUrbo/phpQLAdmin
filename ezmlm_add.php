<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.36.2.3 2005-04-20 17:50:57 turbo Exp $
//
require("./include/pql_session.inc");
require("./include/pql_config.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

if($_REQUEST["add_subs"]) {
	$_REQUEST["subscribercount"]++;
} else {
	if(!$_REQUEST["subscribercount"]) {
		$_REQUEST["subscribercount"] = 0;
	}
}

if($_REQUEST["add_kill"]) {
	$_REQUEST["killcount"]++;
} else {
	if(!$_REQUEST["killcount"]) {
		$_REQUEST["killcount"] = 0;
	}
}

if(!$_REQUEST["domainname"]) {
	// We're not called with a specific domain name to add list to/in

	if($_SESSION["ALLOW_BRANCH_CREATE"]) {
		// This is a 'super-admin' - get ALL branches
		$domains = pql_get_domains($_pql);
	} else {
		// Get list of domains where we are listed in the ezmlmAdministrator attribute
		foreach($_SESSION["BASE_DN"] as $dn)  {
			$dom = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $_SESSION["USER_DN"]);
			if(is_array($dom)) {
				foreach($dom as $d)
				  $domains[] = $d;
			}
		}
	}

    if(!is_array($domains))
	  // if no domain defined - fatal error
	  die("<b>".$LANG->_('Can\'t find any domains!')."</b><br>");
	else {
		// Get the domainname from the domain object
		foreach($domains as $key => $d) {
			$dont_add = 0;
			
			// Get the default domainname for the domain
			$defaultdomainname = pql_get_attribute($_pql->ldap_linkid, $d, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
			
			// Remove duplicates
			if($domain_list) {
				foreach($domain_list as $branch => $data)
				  for($i=0; $data[$i]; $i++)
					if($data[$i] == $defaultdomainname)
					  $dont_add = 1;
			}
			
			if(!$dont_add)
			  $domain_list[$d][] = pql_maybe_idna_decode($defaultdomainname);
		}
		
		foreach($domains as $key => $d) {
			$dont_add = 0;
			
			// Get any additional domainname(s) for the domain
			$additionaldomainnames = pql_get_attribute($_pql->ldap_linkid, $d, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
			
			if(is_array($additionaldomainnames)) {
				foreach($additionaldomainnames as $additional) {
					// Remove duplicates
					if($domain_list) {
						foreach($domain_list as $branch => $data)
						  for($i=0; $data[$i]; $i++)
							if($data[$i] == $additional)
							  $dont_add = 1;
					}
					
					if(!$dont_add)
					  $domain_list[$d][] = pql_maybe_idna_decode($additional);
				}
			}
		}
	}
} else {
	// We're called with a specific domain name to add list to/in

	// Get domain branch and root DN(s)
	if(!$_REQUEST["rootdn"] and !$_REQUEST["domain"] and $_REQUEST["domainname"]) {
		$tmp = split(';', $_REQUEST["domainname"]);

		$_REQUEST["domain"]     = $tmp[0];
		$_REQUEST["domainname"] = $tmp[1];

		$_REQUEST["rootdn"] = pql_get_rootdn($_REQUEST["domain"]);

		$url["domain"] = pql_format_urls($_REQUEST["domain"]);
		$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
	}

	// Get basemaildir path for domain
	if(!($basemaildir = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR"))))
	  die(pql_complete_constant($LANG->_('Can\'t get %what% path from domain \'%domain%\'!'),
								array('what'   => pql_get_define("PQL_ATTR_BASEMAILDIR"),
									  'domain' => $_REQUEST["domainname"])));
	
	require($_SESSION["path"]."/include/pql_ezmlm.inc");

	$user  = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
	$ezmlm = new ezmlm($user, $basemaildir);

	// How many domains do we have in this branch/domain?
	$count = $ezmlm->mailing_lists_hostsindex["COUNT"];

	// How many do we allow?
	$max   = pql_get_define("PQL_CONF_MAXIMUM_MAILING_LISTS", $_REQUEST["rootdn"]);
		
	// Incase maximum allowed is NULL, we allow unlimited (count + 1 is a good value :).
	$max = $max ? $max : $count + 1;
	if($count > $max) {
?>
  <span class="title2">
    <?=$LANG->_('Sorry, you have reached the maximum allowed mailinglists in this domain')?><br>
    <?php echo pql_complete_constant($LANG->_('You have %count% mailinglists, but only %allowed% is allowed.'),
									 array('count' => $count, 'allowed' => $max)); ?>

  </span>
<?php
		die();
	}
}

// Create list
if(isset($_REQUEST["submit"])) {
	if($_REQUEST["listname"] and $_REQUEST["domainname"]) {
		// Rewrite the array (from value ' CHECKED' to '1').
		$tmp = $checked; unset($checked);
		foreach($tmp as $option => $value) {
			$checked[$option] = 1;
		}

		if(is_array($_REQUEST["killlist"]))
		  $checked["kill"] = 1;

		if($ezmlm->updatelistentry(1, $_REQUEST["listname"], $_REQUEST["domainname"], $checked)) {
			for($i=1; $i <= $_REQUEST["subscribercount"]; $i++) {
				if($_REQUEST["subscriber"][$i])
				  $ezmlm->subscribe($_REQUEST["listname"], $_REQUEST["subscriber"][$i]);
			}

			for($i=1; $i <= $_REQUEST["killcount"]; $i++) {
				if($_REQUEST["killlist"][$i])
				  $ezmlm->subscribe_kill($_REQUEST["listname"], $_REQUEST["killlist"][$i]);
			}

			$msg  = urlencode("Successfully created list ".$_REQUEST["listname"]."<br>");
		} else
		  $msg  = urlencode($ezmlm->error);

		$url  = "ezmlm_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&domainname=".$_REQUEST["domainname"];
		$url .= "&listno=".$_REQUEST["listname"]."&msg=$msg&rlnb=3";
		pql_header($url);
	} else
	  $error_text["listname"] = $LANG->_('Missing')."<br>";
} else {
	if(!$_REQUEST["add_subs"] and !$_REQUEST["add_kill"]) {
		// These should be reasonable defaults I think...
		// But only the first time page is loaded!
		$checked["subhelp"]		= " CHECKED";	// -hH
		$checked["subjump"]		= " CHECKED";	// -jJ
		$checked["reqaddress"]	= " CHECKED";	// -qQ
		$checked["subonly"]		= " CHECKED";	// -uU
		$checked["prefix"]		= " CHECKED";	// -fF
		$checked["archived"]	= " CHECKED";	// -aA
		$checked["indexed"]		= " CHECKED";	// -iI
		$checked["trailers"]	= " CHECKED";	// -tT
		$checked["public"]		= " CHECKED";	// -p
	} else {
		// This is a reload (adding subscriber or reject address).
		// Set the values CHECKED if they where when submitting...
		if($_REQUEST["archived"])		$checked["archived"]		= " CHECKED";	// -aA
		if($_REQUEST["remotecfg"])		$checked["remotecfg"]		= " CHECKED";	// -cC
		if($_REQUEST["digest"])			$checked["digest"]			= " CHECKED";	// -dD
		if($_REQUEST["prefix"])			$checked["prefix"]			= " CHECKED";	// -fF
		if($_REQUEST["guardedarchive"])	$checked["guardedarchive"]	= " CHECKED";	// -gG
		if($_REQUEST["subhelp"])		$checked["subhelp"]			= " CHECKED";	// -hH
		if($_REQUEST["indexed"])		$checked["indexed"]			= " CHECKED";	// -iI
		if($_REQUEST["subjump"])		$checked["subjump"]			= " CHECKED";	// -jJ
		if($_REQUEST["sublistable"])	$checked["sublistable"]		= " CHECKED";	// -lL
		if($_REQUEST["moderated"])		$checked["moderated"]		= " CHECKED";	// -mM
		if($_REQUEST["modonly"])		$checked["modonly"]			= " CHECKED";	// -oO
		if($_REQUEST["reqaddress"])		$checked["reqaddress"]		= " CHECKED";	// -qQ
		if($_REQUEST["remoteadm"])		$checked["remoteadm"]		= " CHECKED";	// -{rn,RN}
		if($_REQUEST["submoderated"])	$checked["submoderated"]	= " CHECKED";	// -sS
		if($_REQUEST["trailers"])		$checked["trailers"]		= " CHECKED";	// -tT
		if($_REQUEST["subonly"])		$checked["subonly"]			= " CHECKED";	// -uU
		if($_REQUEST["extras"])			$checked["extras"]			= " CHECKED";	// -xX
		if($_REQUEST["pubpriv"] == 'public')
		  $checked["public"]			= " CHECKED";	// -p
		elseif($_REQUEST["pubpriv"] == 'private')
		  $checked["private"]			= " CHECKED";	// -P
	}
}

// --------------------------------------------
// We haven't submitted (or we're missing listname and/or domain name) - show 'add list form'.

require($_SESSION["path"]."/header.html");
if(!$_REQUEST["domainname"]) {
?>
  <span class="title1"><?=$LANG->_('Create mailinglist to system')?></span>
<?php
} else {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create mailinglist in domain %domain%'),
														array('domain' => $_REQUEST["domainname"]))?></span>
<?php
}
?>
  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="addlist">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Base configuration')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('List name')?></td>
          <td><?php echo pql_format_error_span($error_text["listname"]); ?>

<?php
if(!$_REQUEST["domainname"]) {
	// No domain - select box with existing domains
?>
            <input name="listname" value="<?=$_REQUEST["listname"]?>"><b>@<?php
	if(is_array($domain_list)) {
?></b>
            <select name="domainname">
<?php	foreach($domain_list as $branch => $data) {
			for($i=0; $data[$i]; $i++) {
?>
	          <option value="<?=$branch.";".$data[$i]?>"><?=$data[$i]?></option>
<?php
			}
		}
?>
            </select>
<?php
	} else {
		echo "$domain_list</b>\n";
?>
            <input type="hidden" name="domainname" value="<?=$domain_list?>">
<?php
	}
} else {
	// Got domainname, show that (and remember it!)
?>
            <input type="hidden" name="domainname" value="<?=$_REQUEST["domainname"]?>">
            <input name="listname" value="<?=$_REQUEST["listname"]?>"><b>@<?=$_REQUEST["domainname"]?></b>
<?php
}
?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('List owner')?></td>
          <td>
            <input name="listowner" value="<?=$_REQUEST["listowner"]?>">
            <i>(<b><?=$LANG->_('Optional')?></b>: <?=$LANG->_('If not set, mailbox in list directory')?>)</i>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('From address')?></td>
          <td>
            <input name="fromaddress" value="<?=$_REQUEST["fromaddress"]?>">
            <i>(<b><?=$LANG->_('Optional')?></b>: <?=$LANG->_('If not set, same as listname')?>)</i>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Parent list')?></td>
          <td>
            <input name="listparent" value="<?=$_REQUEST["listparent"]?>">
            <i>(<b><?=$LANG->_('Optional')?></b>: <?=$LANG->_('Make the list a sublist of list')?>)</i>
          </td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th valign="top" align="left"><?=$LANG->_('Subscription options')?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subhelp" accesskey="h"<?=$onchg?><?=$checked["subhelp"]?>></td>
              <td class="title"><?=$LANG->_('Subscription verification')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subjump" accesskey="j"<?=$onchg?><?=$checked["subjump"]?>></td>
              <td class="title"><?=$LANG->_('Unsubscription verification')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="submoderated" accesskey="s"<?=$onchg?><?=$checked["submoderated"]?>></td>
              <td class="title"><?=$LANG->_('<u>S</u>ubscription moderation')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="sublistable" accesskey="l"<?=$onchg?><?=$checked["sublistable"]?>></td>
              <td class="title"><?=$LANG->_('Subscriber <u>l</u>istable')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="reqaddress" accesskey="q"<?=$onchg?><?=$checked["reqaddress"]?>></td>
              <td class="title"><?=$LANG->_('Enable re<u>q</u>uest address')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subonly" accesskey="u"<?=$onchg?><?=$checked["subonly"]?>></td>
              <td class="title"><?=$LANG->_('Allow only s<u>u</u>bscribers posts')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="modonly" accesskey="o"<?=$onchg?><?=$checked["modonly"]?>></td>
              <td class="title"><?=$LANG->_('Allow only m<u>o</u>derator posts')?></td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left"><?=$LANG->_('Basic List options')?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="moderated" accesskey="m"<?=$onchg?><?=$checked["moderated"]?>></td>
              <td class="title"><?=$LANG->_('Moderation - <u>M</u>essage')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="prefix" accesskey="f"<?=$onchg?><?=$checked["prefix"]?>></td>
              <td class="title"><?=$LANG->_('Subject pre<u>f</u>ix')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="remotecfg" accesskey="c"<?=$onchg?><?=$checked["remotecfg"]?>></td>
              <td class="title"><?=$LANG->_('Enable remote <u>c</u>onfiguration')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="remoteadm" accesskey="r"<?=$onchg?><?=$checked["remoteadm"]?>></td>
              <td class="title"><?=$LANG->_('Enable <u>r</u>emote administration')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="archived" accesskey="a"<?=$onchg?><?=$checked["archived"]?>></td>
              <td class="title"><?=$LANG->_('<u>A</u>rchived')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="guardedarchive" accesskey="g"<?=$onchg?><?=$guardarch?>></td>
              <td class="title"><?=$LANG->_('<u>G</u>uarded archive')?></td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left"><?=$LANG->_('Extra List options')?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="digest" accesskey="d"<?=$onchg?><?=$checked["digest"]?>></td>
              <td class="title"><?=$LANG->_('Enable <u>d</u>igest')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="indexed" accesskey="i"<?=$onchg?><?=$checked["indexed"]?>></td>
              <td class="title"><?=$LANG->_('<u>I</u>ndexed')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="trailers" accesskey="t"<?=$onchg?><?=$checked["trailers"]?>></td>
              <td class="title"><?=$LANG->_('Add <u>t</u>railers to messages')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="extras" accesskey="x"<?=$onchg?><?=$checked["extras"]?>></td>
              <td class="title"><?=$LANG->_('Enable list e<u>x</u>tras')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="public"<?=$checked["public"]?>></td>
              <td class="title"><?=$LANG->_('<u>P</u>ublic')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="private"<?=$checked["private"]?>></td>
              <td class="title"><?=$LANG->_('<u>P</u>rivate')?></td>
            </tr>
          </th>
        </table>
      </th>
    </table>

    <br><!-- ======================================================= -->

    <!-- add subscribers at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Subscriber address(es)')?></th>
<?php
	for($i = 1; $i <= $_REQUEST["subscribercount"]; $i++) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Subscriber')?></td>
          <td><?=$subscriber[$i]?></td>
        </tr>
        <input type="hidden" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>" size="50">
<?php
	}
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Subscriber')?></td>
          <td><input type="text" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>"<?=$onchg?> size="50"></td>
        </tr>
      </th>
    </table>

    <table>
      <th>
        <tr>
          <td>
            <input type="submit" name="add_subs" value="<?=$LANG->_('Add additional subscriber address')?>">
          </td>
        </tr>
      </th>
    </table>
    <!-- add subscribers at creation -->

    <br><!-- ======================================================= -->

    <!-- add kill at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Rejected address(es)')?></th>
<?php
	for($i = 1; $i <= $_REQUEST["killcount"]; $i++) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Address')?></td>
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>"<?=$onchg?> size="50"></td>
        </tr>

<?php
	}
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Address')?></td>
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>"<?=$onchg?> size="50"></td>
        </tr>
      </th>
    </table>

    <table>
      <th>
        <tr>
          <td>
            <input type="submit" name="add_kill" value="<?=$LANG->_('Add additional reject address')?>">
          </td>
        </tr>
      </th>
    </table>
    <!-- add kill at creation -->

    <br><!-- ======================================================= -->

    <input type="hidden" name="subscribercount" value="<?=$_REQUEST["subscribercount"]?>">
    <input type="hidden" name="killcount"       value="<?=$_REQUEST["killcount"]?>">
    <input type="hidden" name="rootdn"          value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"          value="<?=$_REQUEST["domain"]?>">

    <p>

    <input type="submit" name="submit" value="<?=$LANG->_('Create list')?>">
  </form>
  </body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
