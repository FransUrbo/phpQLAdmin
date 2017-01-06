<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.48 2007-03-14 12:10:51 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

$values = array("archived", "remotecfg", "digest", "prefix", "guardedarchive", "subhelp",
				"indexed", "subjump", "sublistable", "moderated", "modonly", "reqaddress",
				"remoteadm", "submoderated", "trailers", "subonly", "extras", "listparent",
				"fromaddress", "listowner", "public", "private");

if(!empty($_REQUEST["add_subs"])) {
	$_REQUEST["subscribercount"]++;
} else {
	if(empty($_REQUEST["subscribercount"])) {
		$_REQUEST["subscribercount"] = 0;
	}
}

if(!empty($_REQUEST["add_kill"])) {
	$_REQUEST["killcount"]++;
} else {
	if(empty($_REQUEST["killcount"])) {
		$_REQUEST["killcount"] = 0;
	}
}
// }}}

if(empty($_REQUEST["domainname"])) {
	// {{{ We're not called with a specific domain name to add list to/in

	// {{{ Get domains
	if(!empty($_SESSION["ALLOW_BRANCH_CREATE"])) {
		// This is a 'super-admin' - get ALL branches
		$domains = pql_get_domains();
	} else {
		// Get list of domains where we are listed in the ezmlmAdministrator attribute
		foreach($_SESSION["BASE_DN"] as $dn)  {
			$dom = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $_SESSION["USER_DN"]);
			if(is_array($dom)) {
				foreach($dom as $d)
				  $domains[] = $d;
			}
		}
	}
// }}}

    if(!is_array($domains))
	  // if no domain defined - fatal error
	  die("<b>".$LANG->_('Can\'t find any domains!')."</b><br>");
	else {
		// {{{ Get the domainname from the domain object
		foreach($domains as $key => $d) {
			$dont_add = 0;
			
			// Get the default domainname for the domain
			$defaultdomainname = $_pql->get_attribute($d, pql_get_define("PQL_ATTR_DEFAULTDOMAIN"));
			
			// Remove duplicates
			if(!empty($domain_list)) {
				foreach($domain_list as $branch => $data)
				  for($i=0; $i < count($data); $i++)
					if($data[$i] == $defaultdomainname)
					  $dont_add = 1;
			}
			
			if(!$dont_add)
			  $domain_list[$d][] = pql_maybe_idna_decode($defaultdomainname);
		}
		
		foreach($domains as $key => $d) {
			$dont_add = 0;
			
			// Get any additional domainname(s) for the domain
			$additionaldomainnames = $_pql->get_attribute($d, pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
			
			if(is_array($additionaldomainnames)) {
				foreach($additionaldomainnames as $additional) {
					// Remove duplicates
					if(!empty($domain_list)) {
						foreach($domain_list as $branch => $data)
						  for($i=0; $i < count($data); $i++)
							if($data[$i] == $additional)
							  $dont_add = 1;
					}
					
					if(!$dont_add)
					  $domain_list[$d][] = pql_maybe_idna_decode($additional);
				}
			}
		}
// }}}
	}
// }}}
} else {
	// {{{ We're called with a specific domain name to add list to/in

	// {{{ Get domain branch and root DN(s)
	if(empty($_REQUEST["rootdn"]) and empty($_REQUEST["domain"]) and !empty($_REQUEST["domainname"])) {
		$tmp = explode(';', $_REQUEST["domainname"]);

		$_REQUEST["domain"]     = $tmp[0];
		$_REQUEST["domainname"] = $tmp[1];

		$_REQUEST["rootdn"] = pql_get_rootdn($_REQUEST["domain"]);

		$url["domain"] = pql_format_urls($_REQUEST["domain"]);
		$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
	}
// }}}

	// {{{ Get basemaildir path for domain
	if(!($basemaildir = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_BASEMAILDIR"))))
	  die(pql_complete_constant($LANG->_('Can\'t get %what% path from domain \'%domain%\'!'),
								array('what'   => pql_get_define("PQL_ATTR_BASEMAILDIR"),
									  'domain' => $_REQUEST["domainname"])));
// }}}

	require($_SESSION["path"]."/include/pql_ezmlm.inc");
	$user  = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
	$ezmlm = new ezmlm($user, $basemaildir);

	// How many domains do we have in this branch/domain?
	if(!empty($ezmlm->mailing_lists_hostsindex["COUNT"]))
	  $count = $ezmlm->mailing_lists_hostsindex["COUNT"];
	else
	  $count = 0;

	// How many do we allow?
	$max   = pql_get_define("PQL_CONF_MAXIMUM_MAILING_LISTS", $_REQUEST["rootdn"]);
		
	// Incase maximum allowed is NULL, we allow unlimited (count + 1 is a good value :).
	$max = $max ? $max : $count + 1;
	if($count > $max) {
?>
  <span class="title2">
    <?php echo $LANG->_('Sorry, you have reached the maximum allowed mailinglists in this domain')?><br>
    <?php echo pql_complete_constant($LANG->_('You have %count% mailinglists, but only %allowed% is allowed.'),
									 array('count' => $count, 'allowed' => $max)); ?>

  </span>
<?php
		die();
	}
// }}}
}

// {{{ Create list
if(!empty($_REQUEST["submit"])) {
	if(!empty($_REQUEST["listname"]) and !empty($_REQUEST["domainname"])) {
	  $checked["pubpriv"] = $_REQUEST["pubpriv"];
	  foreach($values as $value)
		$checked[$value] = (empty($_REQUEST[$value]) ? 0 : 1);

	  if(is_array($_REQUEST["killlist"])) {
		$checked["kill"] = 1;
	  }
	  
	  if($ezmlm->updatelistentry(1, $_REQUEST["listname"], $_REQUEST["domainname"], $checked)) {
		for($i=1; $i <= count($_REQUEST["subscriber"]); $i++) {
		  if($_REQUEST["subscriber"][$i])
			$ezmlm->subscribe($_REQUEST["listname"], $_REQUEST["subscriber"][$i]);
		}
		
		for($i=1; $i <= count($_REQUEST["killlist"]); $i++)
		  if($_REQUEST["killlist"][$i])
			$ezmlm->subscribe_kill($_REQUEST["listname"], $_REQUEST["killlist"][$i]);
		
		$msg  = urlencode("Successfully created list ".$_REQUEST["listname"]."<br>");
	  } else
		$msg  = urlencode($ezmlm->error);
	  
	  $url  = "ezmlm_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&domainname=".$_REQUEST["domainname"];
	  $url .= "&listno=".$_REQUEST["listname"]."&msg=$msg&rlnb=3";
	  pql_header($url);
	} else
	  $error_text["listname"] = $LANG->_('Missing')."<br>";
} else {
	if(empty($_REQUEST["add_subs"]) and empty($_REQUEST["add_kill"])) {
	  foreach($values as $value)
		$checked[$value] = '';

	  // These should be reasonable defaults I think...
	  // But only the first time page is loaded!
	  $checked["subhelp"]	= " CHECKED";	// -hH
	  $checked["subjump"]	= " CHECKED";	// -jJ
	  $checked["reqaddress"]= " CHECKED";	// -qQ
	  $checked["subonly"]	= " CHECKED";	// -uU
	  $checked["prefix"]	= " CHECKED";	// -fF
	  $checked["archived"]	= " CHECKED";	// -aA
	  $checked["indexed"]	= " CHECKED";	// -iI
	  $checked["trailers"]	= " CHECKED";	// -tT
	  $checked["public"]	= " CHECKED";	// -p
	} else {
	  // This is a reload (adding subscriber or reject address).
	  // Set the values CHECKED if they where when submitting...
	  foreach($values as $value)
		$checked[$value] = (empty($_REQUEST[$value]) ? '' : " CHECKED");	// -aA

	  if($_REQUEST["pubpriv"] == 'public')
		$checked["public"]			= " CHECKED";	// -p
	  elseif($_REQUEST["pubpriv"] == 'private')
		$checked["private"]			= " CHECKED";	// -P
	}
}
// }}}

// {{{ We haven't submitted (or we're missing listname and/or domain name) - show 'add list form'.
require($_SESSION["path"]."/header.html");
if(empty($_REQUEST["domainname"])) {
?>
  <span class="title1"><?php echo $LANG->_('Create mailinglist to system')?></span>
<?php
} else {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create mailinglist in domain %domain%'),
														array('domain' => $_REQUEST["domainname"]))?></span>
<?php
}
?>
  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post" name="addlist">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Base configuration')."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('List name')?></td>
          <td><?php if(!empty($error_text["listname"])) { echo pql_format_error_span($error_text["listname"]); }?>

<?php
if(empty($_REQUEST["domainname"])) {
	// {{{ No domain - select box with existing domains
?>
            <input name="listname" <?php if(!empty($_REQUEST["listname"])) { echo 'value="'.$_REQUEST["listname"].'"'; }?>><b>@<?php
	if(is_array($domain_list)) {
?></b>
            <select name="domainname">
<?php	foreach($domain_list as $branch => $data) {
			for($i=0; $i < count($data); $i++) {
?>
	          <option value="<?php echo $branch.";".$data[$i]?>"><?php echo $data[$i]?></option>
<?php
			}
		}
?>
            </select>
<?php
	} else {
		echo "$domain_list</b>\n";
?>
            <input type="hidden" name="domainname" <?php if(!empty($domain_list)) { echo 'value="'.$domain_list.'"'; }?>>
<?php
	}
// }}}
} else {
	// {{{ Got domainname, show that (and remember it!)
?>
            <input type="hidden" name="domainname" <?php if(!empty($_REQUEST["domainname"])) { echo 'value="'.$_REQUEST["domainname"].'"'; }?>>
            <input name="listname" <?php if(!empty($_REQUEST["listname"])) { echo 'value="'.$_REQUEST["listname"].'"'; }?>><b>@<?php echo $_REQUEST["domainname"]?></b>
<?php
// }}}
}
?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('List owner')?></td>
          <td>
            <input name="listowner" <?php if(!empty($_REQUEST["listowner"])) { echo 'value="'.$_REQUEST["listowner"].'"'; }?>>
            <i>(<b><?php echo $LANG->_('Optional')?></b>: <?php echo $LANG->_('If not set, mailbox in list directory')?>)</i>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('From address')?></td>
          <td>
            <input name="fromaddress" <?php if(!empty($_REQUEST["fromaddress"])) { echo 'value="'.$_REQUEST["fromaddress"].'"'; }?>>
            <i>(<b><?php echo $LANG->_('Optional')?></b>: <?php echo $LANG->_('If not set, same as listname')?>)</i>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Parent list')?></td>
          <td>
            <input name="listparent" <?php if(!empty($_REQUEST["listparent"])) { echo 'value="'.$_REQUEST["listparent"].'"'; }?>>
            <i>(<b><?php echo $LANG->_('Optional')?></b>: <?php echo $LANG->_('Make the list a sublist of list')?>)</i>
          </td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th valign="top" align="left"><?php echo $LANG->_('Subscription options')."\n"?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subhelp" accesskey="h"<?php echo $checked["subhelp"]?>></td>
              <td class="title"><?php echo $LANG->_('Subscription verification')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subjump" accesskey="j"<?php echo $checked["subjump"]?>></td>
              <td class="title"><?php echo $LANG->_('Unsubscription verification')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="submoderated" accesskey="s"<?php echo $checked["submoderated"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>S</u>ubscription moderation')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="sublistable" accesskey="l"<?php echo $checked["sublistable"]?>></td>
              <td class="title"><?php echo $LANG->_('Subscriber <u>l</u>istable')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="reqaddress" accesskey="q"<?php echo $checked["reqaddress"]?>></td>
              <td class="title"><?php echo $LANG->_('Enable re<u>q</u>uest address')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="subonly" accesskey="u"<?php echo $checked["subonly"]?>></td>
              <td class="title"><?php echo $LANG->_('Allow only s<u>u</u>bscribers posts')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="modonly" accesskey="o"<?php echo $checked["modonly"]?>></td>
              <td class="title"><?php echo $LANG->_('Allow only m<u>o</u>derator posts')?></td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left"><?php echo $LANG->_('Basic List options')."\n"?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="moderated" accesskey="m"<?php echo $checked["moderated"]?>></td>
              <td class="title"><?php echo $LANG->_('Moderation - <u>M</u>essage')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="prefix" accesskey="f"<?php echo $checked["prefix"]?>></td>
              <td class="title"><?php echo $LANG->_('Subject pre<u>f</u>ix')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="remotecfg" accesskey="c"<?php echo $checked["remotecfg"]?>></td>
              <td class="title"><?php echo $LANG->_('Enable remote <u>c</u>onfiguration')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="remoteadm" accesskey="r"<?php echo $checked["remoteadm"]?>></td>
              <td class="title"><?php echo $LANG->_('Enable <u>r</u>emote administration')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="archived" accesskey="a"<?php echo $checked["archived"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>A</u>rchived')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="guardedarchive" accesskey="g"<?php echo $checked["guardedarchive"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>G</u>uarded archive')?></td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left"><?php echo $LANG->_('Extra List options')."\n"?>
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="digest" accesskey="d"<?php echo $checked["digest"]?>></td>
              <td class="title"><?php echo $LANG->_('Enable <u>d</u>igest')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="indexed" accesskey="i"<?php echo $checked["indexed"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>I</u>ndexed')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="trailers" accesskey="t"<?php echo $checked["trailers"]?>></td>
              <td class="title"><?php echo $LANG->_('Add <u>t</u>railers to messages')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="checkbox" name="extras" accesskey="x"<?php echo $checked["extras"]?>></td>
              <td class="title"><?php echo $LANG->_('Enable list e<u>x</u>tras')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p" value="public"<?php echo $checked["public"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>P</u>ublic')?></td>
            </tr>

            <tr class="<?php pql_format_table(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p" value="private"<?php echo $checked["private"]?>></td>
              <td class="title"><?php echo $LANG->_('<u>P</u>rivate')?></td>
            </tr>
          </th>
        </table>
      </th>
    </table>

    <br><!-- ======================================================= -->

    <!-- add subscribers at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Subscriber address(es)')."\n"?>
<?php
	for($i=1; $i <= $_REQUEST["subscribercount"]; $i++) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Subscriber')?></td>
          <td><?php echo $_REQUEST["subscriber"][$i]?></td>
        </tr>
        <input type="hidden" name="subscriber[<?php echo $i?>]" <?php if(!empty($_REQUEST["subscriber"][$i])) { echo 'value="'.$_REQUEST["subscriber"][$i].'"'; }?> size="50">
<?php
	}
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Subscriber')?></td>
          <td><input type="text" name="subscriber[<?php echo $i?>]" <?php if(!empty($_REQUEST["subscriber"][$i])) { echo 'value="'.$_REQUEST["subscriber"][$i].'"'; }?> size="50"></td>
        </tr>
      </th>
    </table>

    <table>
      <th>
        <tr>
          <td>
            <input type="submit" name="add_subs" value="<?php echo $LANG->_('Add additional subscriber address')?>">
          </td>
        </tr>
      </th>
    </table>
    <!-- add subscribers at creation -->

    <br><!-- ======================================================= -->

    <!-- add kill at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Rejected address(es)')."\n"?>
<?php
	for($i=1; $i <= $_REQUEST["killcount"]; $i++) {
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Address')?></td>
          <td><?php echo $_REQUEST["killlist"][$i]?></td>
        </tr>
        <input type="hidden" name="killlist[<?php echo $i?>]" <?php if(!empty($_REQUEST["killlist"][$i])) { echo 'value="'.$_REQUEST["killlist"][$i].'"'; }?> size="50">

<?php
	}
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Address')?></td>
          <td><input type="text" name="killlist[<?php echo $i?>]" <?php if(!empty($_REQUEST["killlist"][$i])) { echo 'value="'.$_REQUEST["killlist"][$i].'"'; }?> size="50"></td>
        </tr>
      </th>
    </table>

    <table>
      <th>
        <tr>
          <td>
            <input type="submit" name="add_kill" value="<?php echo $LANG->_('Add additional reject address')?>">
          </td>
        </tr>
      </th>
    </table>
    <!-- add kill at creation -->

    <br><!-- ======================================================= -->

    <input type="hidden" name="subscribercount" <?php if(!empty($_REQUEST["subscribercount"])) { echo 'value="'.$_REQUEST["subscribercount"].'"'; }?>>
    <input type="hidden" name="killcount"       <?php if(!empty($_REQUEST["killcount"])) { echo 'value="'.$_REQUEST["killcount"].'"'; }?>>
    <input type="hidden" name="rootdn"          <?php if(!empty($_REQUEST["rootdn"])) { echo 'value="'.$_REQUEST["rootdn"].'"'; }?>>
    <input type="hidden" name="domain"          <?php if(!empty($_REQUEST["domain"])) { echo 'value="'.$_REQUEST["domain"].'"'; }?>>

    <p>

    <input type="submit" name="submit" value="<?php echo $LANG->_('Create list')?>">
  </form>
  </body>
</html>
<?php
// }}}

pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
