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
QueueInsertAfter (Queue *q, Link *after, Link *l)
{
    l->queue = q;
    l->prev = after;
    l->next = after->next;
    after->next = l;
    l->next->prev = l;
    q->nitems += 1;
}

inline
void
QueueInsertFirst (Queue *q, Link *l)
{
    QueueInsertAfter (q, (Link *)q, l);
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
    if (l->queue->nitems > 0) {
	l->prev->next = l->next;
	l->next->prev = l->prev;
	l->queue->nitems -= 1;
    }
    l->next = NULL;
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

extern void	QueueModuleInit ();
extern void	QueueFreeLink (Link *l);
extern Link	*QueueAllocLink ();
extern void	QueueInit (Queue *q);

#endif	// _queue_h_
