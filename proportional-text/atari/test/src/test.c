/**
 * Proportional font test
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <atari.h>
#include <tgi.h>
#include <pfont.h>
#include <conio.h>

void main(void)
{
  tgi_install(tgi_static_stddrv);
  tgi_init();
  pputs("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Eget aliquet nibh praesent tristique magna. Enim praesent elementum facilisis leo vel fringilla. Quis ipsum suspendisse ultrices gravida dictum fusce ut placerat orci. Lectus nulla at volutpat diam ut. Vitae aliquet nec ullamcorper sit. Quis varius quam quisque id diam vel quam. Dignissim cras tincidunt lobortis feugiat vivamus at augue eget arcu. Lectus magna fringilla urna porttitor rhoncus dolor. Orci dapibus ultrices in iaculis nunc sed augue lacus viverra. Et sollicitudin ac orci phasellus. Arcu non sodales neque sodales ut etiam sit amet nisl.\n\n");

  pputs("Ut tristique et egestas quis ipsum suspendisse ultrices. Quam lacus suspendisse faucibus interdum posuere lorem ipsum dolor. Odio tempor orci dapibus ultrices in iaculis. Turpis egestas sed tempus urna et pharetra pharetra. Tristique nulla aliquet enim tortor at. Blandit volutpat maecenas volutpat blandit aliquam. Ultrices vitae auctor eu augue ut. Risus pretium quam vulputate dignissim suspendisse in est ante in. Id aliquet lectus proin nibh. Libero id faucibus nisl tincidunt eget nullam non nisi est. Sed egestas egestas fringilla phasellus. Adipiscing tristique risus nec feugiat in fermentum posuere. Pharetra vel turpis nunc eget lorem dolor sed viverra. Tristique sollicitudin nibh sit amet commodo nulla facilisi. Sapien nec sagittis aliquam malesuada bibendum.\n\n");

  cgetc();
  tgi_done();
}
