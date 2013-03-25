//
//	queue.h
//
//	Definitions for routines used to manage queues.  The queues are
//	doubly-linked lists so it's easier to add and remove from them.
//	Also, the links themselves have pointers to objects so that objects
//	may be on more than one queue.
//
//     not only that, it also seems like the queue is circular.
//     So do not use NULL check when traversing the queue.  Use
//     nitems instead.

#ifndef	_queue_h_
#define	_queue_h_

#ifndef	NULL
#define	NULL	((void *)0)
#endif

#define	QUEUE_MAX_LINKS		400

typedef struct Link {
  struct Link	*next;
  struct Link *prev;
  struct Queue *queue;
  void	*object;
} Link;

typedef struct Queue {
  struct Link *first;
  struct Link *last;
  int		nitems;
} Queue;

inline
  void
QueueLinkInit (Link *l, void *obj)
{
  l->next = NULL;
  l->object = obj;
}

inline
  Link *
QueueNext (Link *l)
{
  return (l->next);
}

inline
  Link *
QueuePrev (Link *l)
{
  return (l->prev);
}

inline
  Link *
QueueFirst (Queue *q)
{
  return (q->first);
}

inline
  Link *
QueueLast (Queue *q)
{
  return (q->last);
}

inline
  void
QueuePrint(Queue *q) {
  Link *l = q->first;
  int i = 0;

  dbprintf('q', "\tFirst: %p Last: %p\n",q->first,q->last);
  while (l != NULL) {
    dbprintf('q', "\tLink: %p o(%p)\n",l,l->object);
    l = l->next; 
    if (i++ == 10)
        break;
  }

  dbprintf('q', "Done printing links of (%p)\n",q);
}

inline
  void
QueueInsertAfter (Queue *q, Link *after, Link *l)
{
  dbprintf('q', "Inserting link (%p) after (%p)\n",l->object,after);
  l->queue = q;

  if (after) {
    l->prev = after;
    dbprintf('q', "After->%p\n",after->next);
    l->next = after->next;
    after->next = l;
    l->next->prev = l;

    if (q->last == after) {
      q->last = after->next;
    }
  } else if (q->nitems > 0) {
    q->first->prev = l;
    l->next = q->first;
    q->first = l;
    q->first->prev = NULL;
  } else {
    q->first = l;

    if (!q->last)
        q->last = q->first;
  }

  q->nitems += 1;

  if (l->object)
    QueuePrint(q);
}

inline
  void
QueueInsertFirst (Queue *q, Link *l)
{
  QueueInsertAfter (q, NULL, l);
}

inline
  void
QueueInsertLast (Queue *q, Link *l)
{
  QueueInsertAfter (q, QueueLast(q), l);
}

inline
  void
QueueRemove (Link *l)
{
  Queue *q = l->queue;
  dbprintf('q', "Removing entry (%p->%p)\n",l,l->object);
  QueuePrint(q);

  if (q->nitems > 1) {
    if (q->first == l)
      q->first = q->first->next;
    else if (q->last == l) {
      q->last = q->last->prev;
      q->last->next = NULL;
    } else {
      l->prev->next = l->next;
      l->next->prev = l->prev;
    }
  } else {
    q->first = NULL;
    q->last = NULL;
  }

  q->nitems -= 1;

  l->prev = NULL;
  l->next = NULL;

  dbprintf('q', "Removing entry (%p->%p)\n",l,l->object);
  QueuePrint(q);
}

inline
  int
QueueLength (Queue *q)
{
  return (q->nitems);
}

inline
  int
QueueEmpty (Queue *q)
{
  return (QueueLength (q) == 0);
}

inline
void
LinkMoveToLast(Link *l) {
  Queue *q = l->queue;
  QueueRemove(l);

  QueueInsertLast(q, l);
}

extern void	QueueModuleInit ();
extern void	QueueFreeLink (Link *l);
extern Link	*QueueAllocLink ();
extern void	QueueInit (Queue *q);

#endif	// _queue_h_
