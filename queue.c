//
//	queue.c
//
//	Variables for queues, and a routine to initialize the pool of links.
//
//

#include "dlxos.h"
#include "queue.h"

Queue		freeLinks;
static Link	linkpool[QUEUE_MAX_LINKS];

void
QueueModuleInit ()
{
  int		i;

  QueueInit (&freeLinks);
  for (i = 0; i < QUEUE_MAX_LINKS; i++) {
    dbprintf ('i', "Initializing queue link %d.\n", i);
    linkpool[i].next = NULL;
    QueueFreeLink (&(linkpool[i]));
  }
}

void
QueueInit (Queue *q)
{
    q->first = (Link *)q;
    q->last = (Link *)q;
    q->nitems = 0;
}

void
QueueFreeLink (Link *l)
{
    extern Queue	freeLinks;

    // Make sure the link has already been freed!
    ASSERT ((l->next == NULL), "Link not empty");
    QueueInsertLast (&freeLinks, l);
}

Link *
QueueAllocLink ()
{
    extern Queue	freeLinks;
    Link	*l;

    if (! QueueEmpty (&freeLinks)) {
	l = QueueFirst (&freeLinks);
	QueueRemove (l);
    } else {
	l = NULL;
    }
    ASSERT ((l != NULL), "Link not allocated!");
    return (l);
}
