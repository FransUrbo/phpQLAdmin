<?php
// Most of the HTML layout is taken from WebUserPrefs by Patrick Bores.
// http://www.pipegrep.net/webuserprefs/
//
// Retreive existing user values from the LDAP server.
$spamassassin = pql_get_attribute($_pql->ldap_linkid, $_GET["user"], pql_get_define("PQL_ATTR_SPAMASSASSIN"));
if(is_array($spamassassin)) {
	// Avoid duplicates (additions to the array) when we've submitted (and
	// the page is reloaded). The _REQUEST array is filled in correctly with
	// values in the for() loop after this foreach().
	foreach($_REQUEST as $key => $value) {
		if(($key != 'rootdn') and ($key != 'domain') and
		   ($key != 'view') and ($key != 'user') and
		   ($key != 'PHPSESSID'))
		   unset($_REQUEST[$key]);
	}

	for($i=0; $i < count($spamassassin); $i++) {
		unset($values); $values = split(' ', $spamassassin[$i]);
		
		// Convert the flat structure to something that looks like the form.
		// I.e. array if there's a space in the value (exeption: 'subject_tag'),
		//      and flat if there isn't...
		$tag = $values[0];
		if(($tag == 'blacklist_from') or ($tag == 'whitelist_to') or ($tag == 'whitelist_from')) {
			// The reject/allow address come in a specific format in the array:
			// tag, value, tag, value [etc].
			$_REQUEST["addresses"][] = $tag;
			$_REQUEST["addresses"][] = $values[1];
		} else {
			if($values[2]) {
				if($tag == 'subject_tag') {
					// More than one value, but spaces is ok/required.
					$_REQUEST[$tag] = eregi_replace("subject_tag ", "", $spamassassin[$i]);
				} elseif($tag == 'add_header') {
					// Convert into a three dimensional array
					unset($tmp);
					$tmp[0] = $values[1]; // where (all)
					$tmp[1] = $values[2]; // X-Spam-XXX line
					for($j=3; $j < count($values); $j++) {
					  $tmp[2] .= $values[$j];
					  if($values[$j+1])
						$tmp[2] .= " ";
					}

					$_REQUEST[$tag][] = $tmp;
				} elseif($tag == 'rewrite_header') {
					unset($tmp);
					for($j=2; $j < count($values); $j++) {
                      $tmp .= $values[$j];
                      if($values[$j+1])
                        $tmp .= " ";
                    }
					$_REQUEST[rewrite_header] = $tmp;
				} else {
					// We have more than one value (ok_languages etc). Make it
					// a two dimensional array.
					for($j=1; $j < count($values); $j++)
					  $_REQUEST[$tag][] = $values[$j];
				}
			} else {
				// One tag, one value.
				$_REQUEST[$tag] = $values[1];
			}
		}
	}
} else {
  // Not previous information - use defaults.
  $_REQUEST["required_hits"]	= '5';
  $_REQUEST["report_safe"]		= '0';

  $_REQUEST["spam_level_stars"]	= '0';
  $_REQUEST["spam_level_char"]	= '*';

  $_REQUEST["rewrite_subject"]	= '1';
  $_REQUEST["subject_tag"]		= '*****SPAM*****';

  $_REQUEST["ok_languages"]		= array();
}
?>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" accept-charset="UTF-8">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
    <input type="hidden" name="user"   value="<?=$url["user"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="attrib" value="spamassassin">

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left"><?=$LANG->_('Antispam configuration')?>
<?php if(!is_array($spamassassin)) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"></td>
          <td colspan="2" class="helptextc"><?=$LANG->_('No values in user object found, using defaults.')?></td>
        </tr>

<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Filter Sensitivity')?></td>
          <td>
            <center>
              <table>
                <tr>
                  <td>Less Sensitive</td>
                  <td align="right">More Sensitive</td>
                </tr>
    
                <tr>
                  <td colspan="2">
                    <input type="radio" name="required_hits" value="10" <?php if($_REQUEST["required_hits"] == 10) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="9"  <?php if($_REQUEST["required_hits"] ==  9) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="8"  <?php if($_REQUEST["required_hits"] ==  8) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="7"  <?php if($_REQUEST["required_hits"] ==  7) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="6"  <?php if($_REQUEST["required_hits"] ==  6) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="5"  <?php if($_REQUEST["required_hits"] ==  5) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="4"  <?php if($_REQUEST["required_hits"] ==  4) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="3"  <?php if($_REQUEST["required_hits"] ==  3) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="2"  <?php if($_REQUEST["required_hits"] ==  2) { echo "CHECKED"; }?>>&nbsp;
                    <input type="radio" name="required_hits" value="1"  <?php if($_REQUEST["required_hits"] ==  1) { echo "CHECKED"; }?>>&nbsp;
                  </td>
                </tr>
              </table>
            </center>
          </td>
  
          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td><i>This is the <b>required_hits</b>' value, you can adjust how sensitive the filter is by using the scale below. A setting in the middle is usually sufficient.</i></td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Spam report')?></td>
          <td>
            <input type="radio" name="report_safe" value="0" <?php if($_REQUEST["report_safe"] == "0") { echo "CHECKED"; } ?>>Include report in headers of original message<br>
            <input type="radio" name="report_safe" value="1" <?php if($_REQUEST["report_safe"] == "1") { echo "CHECKED"; } ?>>Include original as MIME attachment to report (Default)<br>
            <input type="radio" name="report_safe" value="2" <?php if($_REQUEST["report_safe"] == "2") { echo "CHECKED"; } ?>>Include original as text-only attachment to report<br>
            <input type="checkbox" name="use_terse_report" value="1" <?php if(!@empty($_REQUEST["terse_control"])) { echo "CHECKED"; } ?>>Use abbreviated report<br>
          </td>

          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td><i>When the filter identifies a message as spam, it makes a report of all tests and scores assigned.</i></td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Header control')?></td>
          <td>
            <input type="checkbox" name="always_add_headers" value="1" <?php if(!@empty($_REQUEST["always_add_headers"])) { echo "CHECKED"; } ?>>Add headers to non-spam messages<br>
            <input type="checkbox" name="fold_headers"       value="1" <?php if(!@empty($_REQUEST["fold_headers"])) { echo "CHECKED"; } ?>>Allow multiple lines in headers<br>
            <input type="checkbox" name="spam_level_stars"   value="1" <?php if($_REQUEST["spam_level_stars"]) { echo "CHECKED"; } ?>>Use "Spam Level" header, using the character <input type="text" name="spam_level_char" value="<?php if($_REQUEST["spam_level_char"]) { echo $_REQUEST["spam_level_char"]; } else { echo "*"; }?>" size="1">
          </td>
  
          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td><i>The filter may add certain headers to your messages. You can control this behavior here.</i></td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Subject rewrite')?></td>
          <td>
            <input type="checkbox" name="rewrite_subject" <?php if($_REQUEST["rewrite_subject"]) { echo "CHECKED"; }?>>Tag Subject lines, using this text: <input type="text" name="subject_tag" value="<?php if($_REQUEST["subject_tag"]) { echo $_REQUEST["subject_tag"]; } else { echo "*****SPAM*****"; }?>" size="14">
          </td>
  
          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td><i>When a message is identified as spam, the filter can append some text to the subject of the message.</i></td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Allowed languages')?></td>
          <td>
            <select multiple name="ok_languages[]" size="8">
              <option value="">[none]</option>
              <option value="all">[all]</option>
              <option value="af" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('af', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Afrikaans</option>
              <option value="sq" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sq', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Albanian</option>
              <option value="am" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('am', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Amharic</option>
              <option value="ar" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ar', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Arabic</option>
              <option value="hy" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('hy', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Armenian</option>
              <option value="eu" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('eu', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Basque</option>
              <option value="bs" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('bs', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Bosnian</option>
              <option value="bg" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('bg', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Bulgarian</option>
              <option value="be" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('be', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Byelorussian</option>
              <option value="ca" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ca', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Catalan</option>
              <option value="zh" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('zh', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Chinese</option>
              <option value="hr" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('hr', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Croatian</option>
              <option value="cs" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('cs', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Czech</option>
              <option value="da" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('da', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Danish</option>
              <option value="nl" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('nl', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Dutch</option>
              <option value="en" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('en', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>English</option>
              <option value="eo" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('eo', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Esperanto</option>
              <option value="et" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('et', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Estonian</option>
              <option value="fi" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('fi', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Finnish</option>
              <option value="fr" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('fr', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>French</option>
              <option value="fy" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('fy', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Frisian</option>
              <option value="ka" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ka', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Georgian</option>
              <option value="de" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('de', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>German</option>
              <option value="el" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('el', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Greek</option>
              <option value="he" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('he', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Hebrew</option>
              <option value="hi" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('hi', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Hindi</option>
              <option value="hu" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('hu', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Hungarian</option>
              <option value="is" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('is', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Icelandic</option>
              <option value="id" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('id', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Indonesian</option>
              <option value="ga" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ga', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Irish Gaelic</option>
              <option value="it" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('it', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Italian</option>
              <option value="ja" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ja', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Japanese</option>
              <option value="ko" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ko', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Korean</option>
              <option value="la" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('la', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Latin</option>
              <option value="lv" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('lv', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Latvian</option>
              <option value="lt" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('lt', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Lithuanian</option>
              <option value="ms" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ms', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Malay</option>
              <option value="mr" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('mr', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Marathi</option>
              <option value="ne" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ne', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Nepali</option>
              <option value="no" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('no', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Norwegian</option>
              <option value="fa" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('fa', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Persian</option>
              <option value="pl" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('pl', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Polish</option>
              <option value="pt" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('pt', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Portuguese</option>
              <option value="qu" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('qu', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Quechua</option>
              <option value="rm" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('rm', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Rhaeto-Romance</option>
              <option value="ro" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ro', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Romanian</option>
              <option value="ru" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ru', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Russian</option>
              <option value="sa" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sa', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Sanskrit</option>
              <option value="sco" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sco', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Scots</option>
              <option value="gd" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('gd', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Scottish Gaelic</option>
              <option value="sr" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sr', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Serbian</option>
              <option value="sk" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sk', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Slovak</option>
              <option value="sl" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sl', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Slovenian</option>
              <option value="es" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('es', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Spanish</option>
              <option value="sw" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sw', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Swahili</option>
              <option value="sv" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('sv', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Swedish</option>
              <option value="tl" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('tl', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Tagalog</option>
              <option value="ta" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('ta', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Tamil</option>
              <option value="th" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('th', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Thai</option>
              <option value="tr" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('tr', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Turkish</option>
              <option value="uk" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('uk', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Ukrainian</option>
              <option value="vi" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('vi', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Vietnamese</option>
              <option value="cy" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('cy', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Welsh</option>
              <option value="yi" <?php if(is_array($_REQUEST["ok_languages"]) and in_array('yi', $_REQUEST["ok_languages"])) { echo "SELECTED"; } ?>>Yiddish</option>
            </select>
          </td>
  
          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td>
                    <i>
                      Select all the languages you expect to receive e-mail in. Any messages received that are written
                      in languages not checked will be considered possible spam in a foreign language.
                    </i>
                  </td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
  
        <tr class="<?php pql_format_table(); ?>">
          <td class="title" valign="top"><?=$LANG->_('Network tests')?></td>
          <td>
            <input type="checkbox" name="use_razor1" value="1" <?php if(!@empty($_REQUEST["use_razor1"])) { echo "CHECKED"; } ?>>Use <a href="http://razor.sourceforge.net" target="_new">Razor v1</a><br>
            <input type="checkbox" name="use_razor2" value="1" <?php if(!@empty($_REQUEST["use_razor2"])) { echo "CHECKED"; } ?>>Use <a href="http://razor.sourceforge.net" target="_new">Razor v2</a><br>
            <input type="checkbox" name="use_pyzor"  value="1" <?php if(!@empty($_REQUEST["use_pyzor"]))  { echo "CHECKED"; } ?>>Use <a href="http://pyzor.sourceforge.net" target="_new">Pyzor</a><br>
            <input type="checkbox" name="use_dcc"    value="1" <?php if(!@empty($_REQUEST["use_dcc"]))    { echo "CHECKED"; } ?>>Use <a href="http://www.dcc-servers.net/dcc/" target="_new">DCC</a> (Distributed Checksum Clearinghouse)<br>
            <input type="checkbox" name="skip_rbl_checks" value="0" <?php if(@$_REQUEST["skip_rbl_checks"] == "0") { echo "CHECKED"; } ?>>Use <a href="http://mail.abuse.net/rbl" target="_new">RBL</a> (Realtime Blackhole List)<br>
          </td>
  
          <td valign="top">
            <table>
              <th>
                <tr>
                  <td valign="top"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td><i>Some checks use internet-based databases to identify spam. Check the ones you want to use.</i></td>
                </tr>
              </th>
            </table>
          </td>
        </tr>
      </th>

      <th colspan="2" align="left"><?=$LANG->_('Custom filter rules')."\n"?>
<?php $i=0;
	  if(@is_array($_REQUEST["addresses"])) {
		for(; $i < count($_REQUEST["addresses"]);) {
?>
        <!-- <?=$_REQUEST["addresses"][$i]?>:<?=$_REQUEST["addresses"][$i+1]?> -->
        <tr class="<?php pql_format_table(); ?>">
<?php		switch($_REQUEST["addresses"][$i]) {
				case "whitelist_from":
					echo "          <td><font color=\"007700\">".$LANG->_('Accept')."</font> ".$LANG->_('Mail From')."</td>\n";
					break;
				case "whitelist_to":
					echo "          <td><font color=\"007700\">".$LANG->_('Accept')."</font> ".$LANG->_('Mail Sent To')."</td>\n";
					break;
				case "blacklist_from":
					echo "          <td><font color=\"FF0000\">".$LANG->_('Reject')."</font> ".$LANG->_('Mail From')."</td>\n";
					break;
			}
?>
          <td><?=$_REQUEST["addresses"][$i+1]?></td>
          <td></td>
        </tr>
        <input type="hidden" name="addresses[<?=$i?>]" value="<?=$_REQUEST["addresses"][$i]?>">
        <input type="hidden" name="addresses[<?=$i+1?>]" value="<?=$_REQUEST["addresses"][$i+1]?>">

<?php		$i = $i+2;
		}
	  }

	  if(@$_REQUEST["key"] and @$_REQUEST["email"]) {
?>
        <!-- <?=$_REQUEST["key"]?>:<?=$_REQUEST["email"]?> -->
        <tr class="<?php pql_format_table(); ?>">
<?php	switch($_REQUEST["key"]) {
			case "whitelist_from":
				echo "          <td><font color=\"007700\">Accept</font> Mail From</td>\n";
				break;
			case "whitelist_to":
				echo "          <td><font color=\"007700\">Accept</font> Mail Sent To</td>\n";
				break;
			case "blacklist_from":
				echo "          <td><font color=\"FF0000\">Reject</font> Mail From</td>\n";
				break;
		}
?>
          <td><?=$_REQUEST["email"]?></td>
          <td></td>
        </tr>
        <input type="hidden" name="addresses[<?=$i++?>]" value="<?=$_REQUEST["key"]?>">
        <input type="hidden" name="addresses[<?=$i++?>]" value="<?=$_REQUEST["email"]?>">

<?php } ?>
        <!-- Add a new rejected/allowed email address!? -->
        <tr class="<?php pql_format_table(); ?>">
          <td>
            <select name="key">
              <option value="whitelist_from">Accept Mail From</option>
              <option value="blacklist_from">Reject Mail From</option>
              <option value="whitelist_to">Accept Mail Sent To</option>
            </select>
          </td>
          <td><input type="text" name="email" size="50"></td>
          <td align="right"><input type="submit" name="addrule" value="<?=$LANG->_('Add Rule')?>"></td>
        </tr>

        <tr colspan="1">
          <td><input type="submit" name="commit" value="<?=$LANG->_('Commit changes to LDAP')?>"></td>
        </tr>
      </th>
    </table>

    <!-- Add header to mail(s) -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left"><?=$LANG->_('Add header line to mail')."\n"?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Where')?></td>
          <td class="title"><?=$LANG->_('X-Spam-???')?></td>
          <td class="title"><?=$LANG->_('What')?></td>
          <td class="title"></td>
        </tr>

<?php for($i=0; $_REQUEST["add_header"][$i]; $i++) {
		$spamassassin = "add_header"." ".$_REQUEST["add_header"][$i][0]." ".$_REQUEST["add_header"][$i][1]." ".$_REQUEST["add_header"][$i][2];
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?=$_REQUEST["add_header"][$i][0]?></td>
          <td><?=$_REQUEST["add_header"][$i][1]?></td>
          <td><?=$_REQUEST["add_header"][$i][2]?></td>
          <td><a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=add_header&user=<?=$url["user"]?>&oldvalue=<?=$spamassassin?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a>&nbsp;&nbsp;<a href="user_del_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=add_header&user=<?=$url["user"]?>&oldvalue=<?=$spamassassin?>&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete line')?>" border="0"></a></td>
        </tr>
<?php } ?>

        <tr class="<?php pql_format_table(); ?>">
          <td></td>
          <td><a href="user_add_attribute.php?rootdn=<?=$url["rootdn"]?>&attrib=add_header&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width\="12" height="12" alt="" border="0"><?=$LANG->_('Add line')?></a></td>
          <td></td>
          <td></td>
        </tr>
      </th>
    </table>
  </form>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
