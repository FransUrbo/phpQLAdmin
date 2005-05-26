<?php
// Just because we're used (almost identically) from both
// domain details _and_ control details, we have this one file
// which just includes the REAL logic so I don't have to duplicate
// code so much...

require($_SESSION["path"]."/include/attrib.control.simscan.inc");
if($_REQUEST["mxhost"]) {
  simscan_init($_REQUEST["mxhost"]);
  simscan_print_view($_REQUEST["mxhost"]);
} else {
  simscan_init($_REQUEST["domain"]);
  simscan_print_view($_REQUEST["domain"]);
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
